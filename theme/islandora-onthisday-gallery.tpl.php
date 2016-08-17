<?php

/**
 * @file
 * Islandora-onthisday-gallery.tpl.php.
 *
 * Borrows markup from the output of the Basic Collection Solution Pack.
 */
?>

<div class="islandora islandora-onthisday-grid">
<?php if (!count($islandora_objects)): ?>
  <p class="no-results"><?php print t('Sorry, there are no objects with a date that is "on this day".'); ?></p>
<?php else: ?>
  <div class="islandora-basic-collection-grid clearfix">
  <?php foreach($islandora_objects as $item): ?>
    <dl class="islandora-basic-collection-object">
        <dt class="islandora-basic-collection-thumb"><a href="<?php print filter_xss($item['title_link']); ?>"><img src="<?php print $item['thumb_link']; ?>" /></a></dt>
        <dd class="islandora-basic-collection-caption"><a href="<?php print filter_xss($item['title_link']); ?>"><?php print $item['label']; ?></a> (<?php print $item['year']; ?>)
        <?php foreach($item['collections'] as $pid => $label): ?>
          <!-- @todo: Render links to collections. ->>
        <?php endforeach; ?>
        </dd>
    </dl>
  <?php endforeach; ?>

  </div>
  <?php endif; ?>
</div>
