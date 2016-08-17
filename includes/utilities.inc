<?php

/**
 * @file
 * Utilities.
 */

/**
 * Assembles a query to send to Solr.
 *
 * @param string $date
 *   The date to use in the query. If FALSE, uses today's date.
 *
 * @return array
 *   An array containing all of the PIDs matching $query.
 */
function islandora_onthisday_get_objects($date = FALSE) {
  if (variable_get('islandora_onthisday_cache_data', 0)) {
    $todays_cache_cid = 'islandora_onthisday:' . date("Y-m-d");
    if ($cached = cache_get($todays_cache_cid)) {
      return $cached->data;
    }
  }

  // Build the Solr query.
  if (!$date) {
    // Get today's month and day.
    $date = date("m-d");
  }
  $fields = explode(',', variable_get('islandora_onthisday_date_fields', 'mods_originInfo_dateIssued_mdt,mods_originInfo_dateCreated_mdt'));
  $query = '';
  $query_parts = array();
  foreach ($fields as $field) {
    // We query for *-mm-dd to get today's month and day.
    $query_parts[] = trim($field) . ':*-' . $date;
  }

  if (count($query_parts) > 1) {
    $query = implode(' OR ', $query_parts);
  }
  else {
    $query = $query_parts[0];
  }

  // Get the object data from the Solr query.
  $objects = islandora_onthisday_query_solr($query);

  if (variable_get('islandora_onthisday_cache_data', 0)) {
    if (!$cached = cache_get($todays_cache_cid)) {
      // It makes more sense to make this cache CACHE_TEMPORARY, but
      // it looks like cron clears all the CACHE_TEMPORARY caches. So
      // if we don't make it permanent, *today's* wiped out, which
      // we don't want.
      cache_set($todays_cache_cid, $objects, 'cache', CACHE_PERMANENT);
    }
  }
  return $objects;
}

/**
 * Queries Islandora's Solr index to get a list of PIDs.
 *
 * @param string $query
 *   The query to send to Solr.
 *
 * @return array
 *   An array of arrays containing attributes for objects matching $query.
 */
function islandora_onthisday_query_solr($query) {
  $objects = array();
  $query_processor = new IslandoraSolrQueryProcessor();
  $query_processor->solrQuery = $query;
  // Some silly number, so we get them all.
  $query_processor->solrLimit = '1000000';
  $fields_to_return = 'PID,fgs_label_s';
  $fields_to_return .= ',' . variable_get('islandora_onthisday_date_fields', 'mods_originInfo_dateIssued_mdt,mods_originInfo_dateCreated_mdt');
  $collection_field = variable_get('islandora_solr_member_of_collection_field', 'RELS_EXT_isMemberOfCollection_uri_ms');
  $fields_to_return .= ',' . $collection_field;
  $query_processor->solrParams['fl'] = $fields_to_return;
  $query_processor->solrParams['sort'] = variable_get('islandora_onthisday_sort_field', 'mods_originInfo_dateIssued_dt');
  $query_processor->executeQuery(FALSE);
  if ($query_processor->islandoraSolrResult['response']['numFound'] > 0) {
    $date_fields = explode(',', variable_get('islandora_onthisday_date_fields', 'mods_originInfo_dateIssued_mdt,mods_originInfo_dateCreated_mdt'));
    foreach ($query_processor->islandoraSolrResult['response']['objects'] as $object) {
      $pid = $object['solr_doc']['PID'];

      // Get a year to display. Ugly.
      $year = '';
      foreach ($date_fields as $date_field) {
        if (isset($object['solr_doc'][$date_field])) {
          if (isset($object['solr_doc'][$date_field][0])) {
            if (preg_match('/^(\d\d\d\d)\-\d\d\-\d\d/',
              $object['solr_doc'][$date_field][0], $matches)) {
              $year = $matches[1];
              break;
            }
          }
        }
      }

      $objects[] = array(
        'pid' => $object['solr_doc']['PID'],
        'label' => $object['solr_doc']['fgs_label_s'],
        'collections' => islandora_onthisday_get_collection_labels($object['solr_doc']),
        'year' => $year,
      );

    }
  }
  return $objects;
}

/**
 * Get a random member of the list of objects from "this day".
 *
 * @return array
 *   An array representing the chosen Islandora object.
 */
function islandora_onthisday_get_random_object() {
  $objects = islandora_onthisday_get_objects();
  if (count($objects)) {
    $random_key = array_rand($objects);
    return $objects[$random_key];
  }
  else {
    return FALSE;
  }
}

/**
 * Clear stale days' worth of data from the cache table.
 *
 * We don't use cache_clear_all() since we don't want to
 * clear today's cache until tomorrow.
 */
function islandora_onthisday_clear_cache() {
  if (!variable_get('islandora_onthisday_cache_data', 0)) {
    return;
  }
  // First, get all the rows in the cache table whose cid starts with
  // 'islandora_onthisday:'.
  $result = db_query("SELECT cid FROM {cache} WHERE cid LIKE 'islandora_onthisday:%'");
  // Then, for each one other than today's, delete that row.
  $todays_cache_cid = 'islandora_onthisday:' . date("Y-m-d");
  foreach ($result as $row) {
    if ($row->cid != $todays_cache_cid) {
      db_delete('cache')
        ->condition('cid', $row->cid)
        ->execute();
    }
  }
}

/**
 * Queries Solr to get labels of collections associated with the object.
 *
 * @param string $doc
 *   The Solr document for an object.
 *
 * @return array
 *   An associative array containing collection PIDs => labels.
 */
function islandora_onthisday_get_collection_labels($doc) {
  $collection_labels = array();
  $collection_field = variable_get('islandora_solr_member_of_collection_field', 'RELS_EXT_isMemberOfCollection_uri_ms');
  $query_processor = new IslandoraSolrQueryProcessor();
  $label = '';
  $pid = $doc['PID'];
  $pid = str_replace('info:fedora/', '', $pid);

  foreach ($doc[$collection_field] as $collection_pid) {
    $query_processor->buildQuery("PID:\"$pid\"");
    $query_processor->solrParams['fl'] = 'PID, ' . $collection_field;
    $query_processor->executeQuery();
    if (!empty($query_processor->islandoraSolrResult) && !empty($query_processor->islandoraSolrResult['response']['objects'])) {
      $label = (!empty($query_processor->islandoraSolrResult['response']['objects'][0]['object_label']) ?
        $query_processor->islandoraSolrResult['response']['objects'][0]['object_label'] : '');
      $collection_labels[$pid] = $label;
    }
  }
  dd($collection_labels, 'Collection labels');
  return $collection_labels;
}
