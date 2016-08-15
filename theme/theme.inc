<?php

/**
 * @file
 * This file contains all theme and preprocess functions.
 */

/**
 * Default template: islandora-onthisday-gallery.tpl.php.
 *
 * @param array $variables
 */
function islandora_onthisday_preprocess_islandora_onthisday_gallery(array &$variables) {
  foreach ($variables['islandora_objects'] as &$object) {
    $object['thumb_link'] = url("islandora/object/" . $object['pid'] . "/datastream/TN/view");
    $object['title_link'] = url("islandora/object/" . $object['pid']);
  }
}
