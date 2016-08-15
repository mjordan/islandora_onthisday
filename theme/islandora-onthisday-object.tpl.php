<?php

/**
 * @file
 * Islandora-onthisday-object.tpl.php.
 */
?>
  <div><a href="<?php print filter_xss($islandora_object['title_link']); ?>"><img src="<?php print $islandora_object['thumb_link']; ?>" /></a></div>
   <div><a href="<?php print filter_xss($islandora_object['title_link']); ?>"><?php print $islandora_object['label']; ?></a> (<?php print $islandora_object['year']; ?>)</div>
