<?php

/**
 * @file
 * Utilities.
 */

/**
 * Queries Islandora's Solr index to get a list of PIDs.
 *
 * The object is the one corresponding to the alias and pointer.
 *
 * @param string $query
 *   The query to send to Solr.
 *
 * @return array
 *   An array containing all of the PIDs matching $query.
 */
function islandora_onthisday_query_solr($query) {
  $pids = array();
  $query_processor = new IslandoraSolrQueryProcessor();
  $query_processor->solrQuery = $query;
  // Some silly number.
  $query_processor->solrLimit = '1000000';
  $query_processor->executeQuery(FALSE);
  if ($query_processor->islandoraSolrResult['response']['numFound'] > 0) {
    foreach ($query_processor->islandoraSolrResult['response']['objects'] as $object) {
      $pids[] = $object['solr_doc']['PID'];
    }
  }
  return $pids;
}
