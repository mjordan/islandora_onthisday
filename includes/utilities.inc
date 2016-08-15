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

  // When caching is available, this function will rebuild the cache
  // from cron or via the drush script.
  return $objects;
}

/**
 * Queries Islandora's Solr index to get a list of PIDs.
 *
 * @param string $query
 *   The query to send to Solr.
 *
 * @return array
 *   An array containing all of the PIDs matching $query.
 */
function islandora_onthisday_query_solr($query) {
  $objects = array();
  $query_processor = new IslandoraSolrQueryProcessor();
  $query_processor->solrQuery = $query;
  // Some silly number, so we get them all.
  $query_processor->solrLimit = '1000000';
  // Get one collection for now, @todo: get all collections.
  $fields_to_return = 'PID,fgs_label_s,RELS_EXT_isMemberOfCollection_uri_s';
  $fields_to_return .= ',' . variable_get('islandora_onthisday_date_fields', 'mods_originInfo_dateIssued_mdt,mods_originInfo_dateCreated_mdt');
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
        'collection' => $object['solr_doc']['RELS_EXT_isMemberOfCollection_uri_s'],
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
