<?php

/**
 * @file
 * islandora-onthisday-gallery.tpl.php
 */
?>

<div class="islandora islandora-onthisday-grid">
  <div class="islandora-basic-collection-grid clearfix">
  <!-- @todo: add count on islandora_objects and if 0, display message to user. -->
  <?php foreach($islandora_objects as $item): ?>
    <dl class="islandora-basic-collection-object">
        <dt class="islandora-basic-collection-thumb"><a href="<?php print filter_xss($item['title_link']); ?>"><img src="<?php print $item['thumb_link']; ?>" /></a></dt>
        <dd class="islandora-basic-collection-caption"><a href="<?php print filter_xss($item['title_link']); ?>"><?php print $item['label']; ?></a></dd>
    </dl>
  <?php endforeach; ?>
</div>
</div>
