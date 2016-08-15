<?php

/**
 * @file
 * Utilities.
 */

/**
 * Assembles a query to send to Solr.
 *
 * @todo: Cache results.
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
    $date = date("Y-m-d");
  }
  $fields = explode(',', variable_get('islandora_onthisday_date_fields', 'mods_originInfo_dateIssued_mdt,mods_originInfo_dateCreated_mdt'));
  $query = '';
  $query_parts = array();
  foreach ($fields as $field) {
    $query_parts[] = trim($field) . ':' . $date;
  }

  if (count($query_parts) > 1) {
    $query = implode(' OR ', $query_parts);
  }
  else {
    $query = $query_parts[0];
  }

  // Get the object data from the Solr query.
  $objects = islandora_onthisday_query_solr($query);

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
  $query_processor->solrParams['fl'] = 'PID,fgs_label_s,RELS_EXT_isMemberOfCollection_uri_s';
  $query_processor->solrParams['sort'] = variable_get('islandora_onthisday_sort_field', 'mods_originInfo_dateIssued_dt');
  $query_processor->executeQuery(FALSE);
  if ($query_processor->islandoraSolrResult['response']['numFound'] > 0) {
    foreach ($query_processor->islandoraSolrResult['response']['objects'] as $object) {
      $pid = $object['solr_doc']['PID'];
      $objects[] = array(
        'pid' => $object['solr_doc']['PID'],
        'label' => $object['solr_doc']['fgs_label_s'],
        'collection' => $object['solr_doc']['RELS_EXT_isMemberOfCollection_uri_s'],
      );
    }
  }
  dd($objects);
  return $objects;
}
