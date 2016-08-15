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
  $items['islandora_onthisday_get_objects'] = array(
    'aliases' => array('iotdgo'),
    'description' => "Get a list of PIDs from Islandora for the objects whose date values in Solr are equal to today's date.",
    'examples' => array(
      'drush islandora_onthisday_get_pids',
    ),
    'options' => array(
      'date' => array(
        'description' => "A date in yyyy-mm-dd format, to be used instead of today's date. Useful for testing.",
      ),
    ),
    'bootstrap' => DRUSH_BOOTSTRAP_DRUPAL_LOGIN,
  );

  return $items;
}

/**
 * Queries Solr for a list of PIDs.
 */
function drush_islandora_onthisday_get_objects() {
  if (!module_exists('islandora_solr')) {
    drush_set_error('SOLR_SEARCH_NOT_AVAILABLE',
      dt('Sorry, Islandora Solr Search not enabled.'));
  }

  // Get the PIDs from the Solr query.
  module_load_include('inc', 'islandora_onthisday', 'includes/utilities');
  if (drush_get_option('date')) {
    $date = drush_get_option('date');
  }
  else {
    $date = NULL;
  }

  $results = islandora_onthisday_get_objects($date);

  if (!count($results) || !$results) {
    drush_set_error('NO_OBJECTS_FOUND',
      dt('Sorry, no objects were found.'));
  }
  else {
    drush_log(dt('!num matching objects found.',
      array('!num' => count($results))), 'ok');
    foreach ($results as $properties) {
      drush_print($properties['pid']);
    }
  }
}
