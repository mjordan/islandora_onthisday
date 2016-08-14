<?php

/**
 * @file
 * Drush file for the Islandora On This Day module.
 */

/**
 * Implements hook_drush_command().
 */
function islandora_onthisday_drush_command() {
  $items = array();
  $items['islandora_onthisday_get_pids'] = array(
    'aliases' => array('iotdgp'),
    'description' => "Get a list of PIDs from Islandora for the objects whose date values in Solr are equal to today's date.",
    'examples' => array(
      'drush islandora_onthisday_get_pids',
    ),
    'bootstrap' => DRUSH_BOOTSTRAP_DRUPAL_LOGIN,
  );

  return $items;
}

/**
 * Queries Solr for a list of PIDs.
 */
function drush_islandora_onthisday_get_pids() {
  if (!module_exists('islandora_solr')) {
    drush_set_error('SOLR_SEARCH_NOT_AVAILABLE',
      dt('Sorry, Islandora Solr Search not enabled.'));
  }

  // Build the Solr query.
  $today = date("Y-m-d");
  $fields = explode(',', variable_get('islandora_onthisday_date_fields', 'mods_originInfo_dateIssued_mdt,mods_originInfo_dateCreated_mdt'));
  $query = '';
  $query_parts = array();
  foreach ($fields as $field) {
    $query_parts[] = trim($field) . ':' . $today;
  }

  if (count($query_parts) > 1) {
    $query = implode(' OR', $query_parts);
  }
  else {
    $query = $query_parts[0];
  }

  // Get the PIDs from the Solr query.
  module_load_include('inc', 'islandora_onthisday', 'includes/utilities');
  $pids = islandora_onthisday_query_solr($query);

  if (!count($pids) || !$pids) {
    drush_set_error('NO_PIDS_FOUND',
      dt('Sorry, no PIDS were found.'));
  }
  else {
    drush_log(dt('!num matching objects found.',
      array('!num' => count($pids))), 'ok');
    foreach ($pids as $pid) {
      drush_print($pid);
    }
  }
}
