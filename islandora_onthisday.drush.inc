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
    'description' => "Generate the list of Islandora objects from Solr whose date values are equal to today's date.",
    'examples' => array(
      'drush islandora_onthisday_get_objects',
    ),
    'options' => array(
      'date' => array(
        'description' => "A date in yyyy-mm-dd format, to be used instead of today's date. Useful for testing.",
      ),
    ),
    'bootstrap' => DRUSH_BOOTSTRAP_DRUPAL_LOGIN,
  );
  $items['islandora_onthisday_clear_cache'] = array(
    'aliases' => array('iotdcc'),
    'description' => "Clear out stale daily caches of 'on this day' data. Does not clear out today's cache.",
    'examples' => array(
      'drush islandora_onthisday_clear_cache',
      'drush islandora_onthisday_clear_cache --today=true',
    ),
    'options' => array(
      'today' => array(
        'description' => "Whether or not to clear today's entries in the cache, if they exist. Default is FALSE.",
      ),
    ),
    'bootstrap' => DRUSH_BOOTSTRAP_DRUPAL_LOGIN,
  );

  return $items;
}

/**
 * Queries Solr for a list of objects.
 *
 * This module queries Solr when a user visits /onthisday. If the caching admin
 * option is set, the query if skipped and the cached version is used. Very
 * large Islandora sites may want to run this command using a Linux cron job
 * to populate the cached version in advance of a user hitting /onthisday.
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

/**
 * Clears the stale data cached by this module.
 *
 * This module clears its cached data using Drupal cron. Large Islandora sites
 * may want to run this command in a Linux cron job if they find that Drupal
 * cron is timing out. This command is also useful during testing and
 * development.
 */
function drush_islandora_onthisday_clear_cache() {
  module_load_include('inc', 'islandora_onthisday', 'includes/utilities');
  $today = drush_get_option('today', FALSE);
  islandora_onthisday_clear_cache($today);
  drush_log(dt('Islandora On This Day cache cleared.', array()), 'ok');
}
