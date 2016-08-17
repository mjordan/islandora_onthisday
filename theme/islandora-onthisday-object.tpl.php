<?php

/**
 * @file
 * Islandora-onthisday-object.tpl.php.
 */
?>
  <div><a href="<?php print filter_xss($islandora_object['title_link']); ?>"><img src="<?php print $islandora_object['thumb_link']; ?>" /></a></div>
   <div><a href="<?php print filter_xss($islandora_object['title_link']); ?>"><?php print $islandora_object['label']; ?></a> (<?php print $islandora_object['year']; ?>)</div>
   <?php if (count($islandora_object['collections'])): ?>
     <div id="islandora-onthisday-collection-info">
     <div id="islandora-onthisday-collections-label"><?php print $islandora_object['collections_label']; ?>:</div>
       <?php foreach($islandora_object['collections'] as $pid => $label): ?>
         <div class="islandora-onthisday-collection-link"><a href="/islandora/object/<?php print $pid; ?>"><?php print filter_xss($label); ?></a></div>
       <?php endforeach; ?>
     </div>
   <?php endif; ?>
