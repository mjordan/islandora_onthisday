<?php

/**
 * @file
 * Islandora-onthisday-gallery.tpl.php.
 *
 * Borrows markup from the output of the Basic Collection Solution Pack.
 */
?>

<a href="/onthisday/rss"><img src="<?php print $rss_icon_path; ?>" class="secondary-display-rss" alt="<?php print $rss_icon_alt; ?>"></a>
<div class="islandora islandora-onthisday-grid">
<?php if (!count($islandora_objects)): ?>
  <p class="no-results"><?php print t('Sorry, there are no objects with a date that is "on this day".'); ?></p>
<?php else: ?>
  <div class="islandora-basic-collection-grid clearfix">
  <div class="islandora-onthisday-header"><?php print filter_xss($header); ?></div>
  <?php foreach($islandora_objects as $item): ?>
    <dl class="islandora-basic-collection-object">
        <dt class="islandora-basic-collection-thumb"><a href="<?php print filter_xss($item['title_link']); ?>"><img src="<?php print $item['thumb_link']; ?>" /></a></dt>
        <dd class="islandora-basic-collection-caption"><a href="<?php print filter_xss($item['title_link']); ?>"><?php print $item['label']; ?></a>
        <?php if (strlen($item['year'])): ?>
        (<?php print $item['year']; ?>)
        <?php endif; ?>
        <?php if (count($item['collections'])): ?>
          <div id="islandora-onthisday-collection-info">
          <div id="islandora-onthisday-collections-label"><?php print $item['collections_label']; ?>:</div>
          <?php foreach($item['collections'] as $pid => $label): ?>
            <div class="islandora-onthisday-collection-link"><a href="/islandora/object/<?php print $pid; ?>"><?php print filter_xss($label); ?></a></div>
          <?php endforeach; ?>
        </div>
        <?php endif; ?>
        </dd>
        <?php if (strlen($item['debugging_metadata'])): ?>
        <dd>
        <pre><?php print $item['debugging_metadata']; ?></pre>
        </dd>
        <?php endif; ?>
    </dl>

  <?php endforeach; ?>

  </div>
  <?php endif; ?>
</div>
