<?php

/**
 * @file
 * Islandora-onthisday-rss-channel.tpl.php.
 *
 * Template for RSS channel for "on this day" items.
 */
?>
<rss version="2.0">
<channel>
  <title><?php print $site_name; ?></title>
  <link>http://localhost:8000</link>
  <description>On this day, <?php print $today; ?></description>
  <language>en</language>
  <copyright />
  <managingEditor />
  <webMaster><?php print $site_mail; ?></webMaster>
<?php if (count($islandora_objects)): ?>
  <?php foreach($islandora_objects as $item): ?>
  <item>
  <?php if (strlen($item['year'])): ?>
    <title><?php print $item['label'] . ' [' . $item['year'] . ']'; ?></title>
  <?php else: ?>
    <title><?php print $item['label']; ?></title>
  <?php endif; ?>
    <link><?php print $item['url']; ?></link>
      <description><img src="<?php print $item['thumb_link']; ?>" title="<?php print $item['description']; ?>" alt="<?php print $item['description']; ?>" /></description>
      <author />
      <guid><?php print $item['url']; ?></guid>
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
