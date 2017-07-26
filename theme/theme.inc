<?php

/**
 * @file
 * This file contains all theme and preprocess functions.
 */

/**
 * Default template: islandora-onthisday-gallery.tpl.php.
 *
 * @param array $variables
 *   The variables passed in from the theming layer.
 */
function islandora_onthisday_preprocess_islandora_onthisday_gallery(array &$variables) {
  $path = drupal_get_path('module', 'islandora_solr_config');
  $variables['rss_icon_path'] = '../' . $path . '/images/rss.png';
  $variables['rss_icon_alt'] = t("RSS feed");
  $variables['header'] = token_replace(variable_get('islandora_onthisday_header', ''));

  foreach ($variables['islandora_objects'] as &$object) {
    $object['thumb_link'] = url("islandora/object/" . $object['pid'] . "/datastream/TN/view");
    $object['title_link'] = url("islandora/object/" . $object['pid']);
    $object['collections_label'] = (count($object['collections']) > 1) ? t('Collections') : t('Collection');
    if (!variable_get('islandora_onthisday_show_collections', 0)) {
      $object['collections'] = array();
    }
  }
}

/**
 * Default template: islandora-onthisday-object.tpl.php.
 *
 * @param array $variables
 *   The variables passed in from the theming layer.
 */
function islandora_onthisday_preprocess_islandora_onthisday_object(array &$variables) {
  $variables['islandora_object']['thumb_link'] = url("islandora/object/" .
    $variables['islandora_object']['pid'] . "/datastream/TN/view");
  $variables['islandora_object']['title_link'] = url("islandora/object/" .
    $variables['islandora_object']['pid']);
}

/**
 * Default template: islandora-onthisday-collection-links.tpl.php.
 *
 * @param array $variables
 *   The variables passed in from the theming layer.
 */
function islandora_onthisday_preprocess_islandora_onthisday_collection_links(array &$variables) {
  $variables['islandora_object']['collections_label'] = (count($variables['islandora_object']['collections']) > 1) ? t('Collections') : t('Collection');
  if (!variable_get('islandora_onthisday_show_collections', 0)) {
    $variables['islandora_object']['collections'] = array();
  }
}

/**
 * Default template: islandora-onthisday-rss-channel.tpl.php.
 *
 * @param array $variables
 *   The variables passed in from the theming layer.
 */
function islandora_onthisday_preprocess_islandora_onthisday_rss_channel(array &$variables) {
  global $base_url;
  global $language;
  $variables['base_url'] = $base_url;
  $variables['site_name'] = variable_get('site_name', 'Drupal');
  $variables['site_mail'] = variable_get('site_mail', 'admin@example.com');
  $variables['today'] = date('F j');
  $variables['language'] = $language->language;
  foreach ($variables['islandora_objects'] as &$item) {
    $islandora_object = islandora_object_load($item['pid']);
    if ($islandora_object['TN']) {
      $tn = $islandora_object['TN'];
      $item['thumb_size'] = $tn->size;
      $item['mimetype'] = $tn->mimetype;
    }
    $item['thumb_link'] = url("$base_url/islandora/object/" . $item['pid'] . "/datastream/TN/view");
    $item['url'] = url("$base_url/islandora/object/" . $item['pid']);

    $dc_xml = $islandora_object['DC']->content;
    $dom = new DOMDocument();
    $dom->loadXML($dc_xml);
    $xpath = new DOMXPath($dom);
    $xpath->registerNamespace('dc', 'http://purl.org/dc/elements/1.1/');
    $descriptions = $xpath->query("//dc:description");
    if ($descriptions->length > 0) {
      $description = $descriptions->item(0)->nodeValue;
    }
    else {
      $description = '[No description available]';
    }

    $item['description'] = htmlspecialchars($description);
  }
}
