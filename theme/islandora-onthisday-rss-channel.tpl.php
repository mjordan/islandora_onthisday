<?php

/**
 * @file
 * Islandora-onthisday-rss-channel.tpl.php.
 *
 * Template for RSS channel for "on this day" items.
 */
?>

<?xml version="1.0" encoding="utf-8"?>
<rss version="2.0">
<channel>
  <title><?php print $site_name; ?></title>
  <link>http://localhost:8000</link>
  <description>On this day, <?php print $today; ?></description>
  <language>en</language>
  <copyright />
  <managingEditor />
  <webMaster />
<?php if (count($islandora_objects)): ?>
  <?php foreach($islandora_objects as $item): ?>
  <item>
      <?php if (strlen($item['year'])): ?>
      <title><?php print $item['label'] . ' [' . $item['year'] . ']'; ?></title>
      <?php else: ?>
      <title><?php print $item['label']; ?></title>
      <?php endif; ?>
      <link><?php print $item['url']; ?></link>
      <description></description>
      <author />
      <guid isPermaLink="false"><?php print $item['pid']; ?></guid>
      <pubDate />
      <category />
      <comments />
      <source url="<?php print $base_url; ?>"><?php print $site_name; ?></source>
      <enclosure url="<?php print $item['thumb_link']; ?>" length="<?php print $item['thumb_size']; ?>" type="<?php print $item['mimetype']; ?>" />
    </item>
  <?php endforeach; ?>
<?php endif; ?>
</channel>
</rss>
