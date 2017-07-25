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
 *   An array of arrays containing attributes for objects matching $query.
 */
function islandora_onthisday_get_objects($date = FALSE) {
  if (variable_get('islandora_onthisday_cache_data', 0)) {
    $todays_cache_cid = 'islandora_onthisday:' . date("m-d");
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

  if (variable_get('islandora_onthisday_set_limit', 0)) {
    $objects = islandora_onthisday_subset_objects($objects);
  }

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

  $fields_to_return = 'PID,fgs_label_s,ancestors_mt,RELS_EXT_hasModel_uri_mt';
  $fields_to_return .= ',' . variable_get('islandora_onthisday_date_fields', 'mods_originInfo_dateIssued_mdt,mods_originInfo_dateCreated_mdt');
  $collection_field = variable_get('islandora_solr_member_of_collection_field', 'RELS_EXT_isMemberOfCollection_uri_ms');

  if (strlen(variable_get('islandora_onthisday_sort_field', ''))) {
    $sort_field = variable_get('islandora_onthisday_sort_field', '');
    $sort_field_bare = preg_replace('/\s+(asc|desc)/', '', $sort_field);
    $fields_to_return .= ',' . $collection_field . ',' . $sort_field_bare;
  }
  else {
    $sort_field = variable_get('islandora_onthisday_sort_field', '');
    $fields_to_return .= ',' . $collection_field;
  }

  $query_processor->solrParams['fl'] = $fields_to_return;
  $query_processor->solrParams['sort'] = $sort_field;
  $query_processor->solrLimit = '1000000';

  $filters = array();
  if (strlen(variable_get('islandora_onthisday_filter_queries', ''))) {
    $filter_queries = preg_split("/\\r\\n|\\r|\\n/", variable_get('islandora_onthisday_filter_queries', ''));
    foreach ($filter_queries as &$filter) {
      $filter = trim($filter);
      $filter = '-' . $filter;
      $filters[] = $filter;
    }
  }

//  Add excluded CModels to Filter list.
  $excluded_cmodels = array();
  $excluded_cmodels = explode(",", variable_get('islandora_onthisday_exclude_cmodels', 'islandora:newspaperPageCModel,islandora:pageCModel'));

  foreach ($excluded_cmodels as $excluded_cmodel) {
   $excluded_cmodel = str_replace(":", "\:", $excluded_cmodel);
   $filter = '-' . 'RELS_EXT_hasModel_uri_mt:info\:fedora/' . $excluded_cmodel;
   $filters[] = $filter;
  }

  if (count($filters)) {
    $query_processor->solrParams['fq'] = implode(' OR ', $filters);
  }

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

      $debugging_metadata = '';
      if (variable_get('islandora_onthisday_show_debugging_metadata', 0) && user_has_role(3)) {
        $debugging_metadata = var_export($object['solr_doc'], TRUE);
      }

      $objects[] = array(
        'pid' => $object['solr_doc']['PID'],
        'label' => $object['solr_doc']['fgs_label_s'],
        'collections' => islandora_onthisday_get_collection_labels($object['solr_doc']),
        'year' => $year,
        'debugging_metadata' => $debugging_metadata,
      );

    }
  }
  if (!strlen(variable_get('islandora_onthisday_sort_field', ''))) {
    usort($objects, 'islandora_onthisday_sort_entries_by_year');
  }
  return $objects;
}

/**
 * Sorts the list of entries by year value.
 *
 * @param string $a
 *   The first string as defined by usort()'s value_compare_func parameter.
 * @param string $b
 *   The second string as defined by usort()'s value_compare_func parameter.
 */
function islandora_onthisday_sort_entries_by_year($a, $b) {
  return strnatcmp($a['year'], $b['year']);
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
 * @param bool $today
 *   Whether or not to clear today's cache entry.
 */
function islandora_onthisday_clear_cache($today = FALSE) {
  if (!variable_get('islandora_onthisday_cache_data', 0)) {
    return;
  }
  if ($today) {
    clear_cache_all('islandora_onthisday', 'cache', TRUE);
    return;
  }

  // If we don't want to clear today's entry, first we get all the rows
  // in the cache table whose cid starts with 'islandora_onthisday:'.
  $result = db_query("SELECT cid FROM {cache} WHERE cid LIKE 'islandora_onthisday:%'");
  // Then, for each one other than today's, delete that row.
  $todays_cache_cid = 'islandora_onthisday:' . date("m-d");
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
  if (!variable_get('islandora_onthisday_show_collections', 0)) {
    return $collection_labels;
  }

  $collection_field = variable_get('islandora_solr_member_of_collection_field', 'RELS_EXT_isMemberOfCollection_uri_ms');

  // If the solr document for the object doesn't contain the
  // collection field, bail.
  if (!isset($doc[$collection_field])) {
    return array();
  }

  // Support DGI's Solr config's ancestor indexing.
  if (isset($doc['ancestors_mt'])) {
    $collections = $doc['ancestors_mt'];
    $collections_to_exclude = explode(',', variable_get('islandora_onthisday_exclude_collections', 'islandora:root'));
    $collections = array_diff($collections, $collections_to_exclude);
  }
  else {
    // If no ancestor indexing, use configured collection field.
    $collections = $doc[$collection_field];
  }

  foreach ($collections as $pid) {
    $pid = str_replace('info:fedora/', '', $pid);
    $pid = preg_replace('/:/', '\\:', $pid);
    $query_processor = new IslandoraSolrQueryProcessor();
    $query_processor->buildQuery("PID:\"$pid\" AND RELS_EXT_hasModel_uri_mt:info\:fedora/islandora\:collectionCModel");
    $query_processor->executeQuery();
    if (!empty($query_processor->islandoraSolrResult) && !empty($query_processor->islandoraSolrResult['response']['objects'])) {
      $label = (!empty($query_processor->islandoraSolrResult['response']['objects'][0]['object_label']) ?
        $query_processor->islandoraSolrResult['response']['objects'][0]['object_label'] : '');
      $clean_pid = preg_replace('/\\\/', '', $pid);
      $collection_labels[$clean_pid] = $label;
    }
  }
  return $collection_labels;
}

/**
 * Subsets the number of objects to display in gallery to a smaller number.
 *
 * @param array $data
 *   The arrary of object data to subset.
 *
 * @return array
 *   The reduced list of object data. List is ramdomized but
 *   sort order of original array is preserved.
 */
function islandora_onthisday_subset_objects(array $data) {
  $config_limit = variable_get('islandora_onthisday_max_objects', '50');
  if (count($data) <= $config_limit) {
    return $data;
  }

  $range_limit = count($data) - 1;
  $rand_pool = range(0, $range_limit);
  $subset_rand_pool = array_rand($rand_pool, $config_limit);
  if (is_int($subset_rand_pool)) {
    $subset_rand_pool = array(1);
  }

  $reduced = array();
  foreach ($data as $key => $value) {
    if (in_array($key, $subset_rand_pool)) {
      $reduced[] = $value;
    }
  }
  return $reduced;
}
