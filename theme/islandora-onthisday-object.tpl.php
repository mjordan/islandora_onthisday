<?php

/**
 * @file
 * Islandora-onthisday-object.tpl.php.
 *
 * Used only in the block, not the gallery.
 */
?>
  <div><a href="<?php print filter_xss($islandora_object['title_link']); ?>"><img src="<?php print $islandora_object['thumb_link']; ?>" /></a></div>
  <div><a href="<?php print filter_xss($islandora_object['title_link']); ?>"><?php print $islandora_object['label']; ?></a>
  <?php if (strlen($islandora_object['year'])): ?>
    (<?php print $islandora_object['year']; ?>)
  <?php endif; ?>
  </div>
