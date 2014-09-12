<?php

?>
<div id="node-<?php print $node->nid; ?>" class="node-artgallery">
  <?php
    $imgcount = count($node->field_img['und']);
    for ($i = 0; $i < $imgcount; $i++) {
      $image_uri = $node->field_img['und'][$i]['uri'];
      // url
      $masthead_raw = image_style_url('artgallery-thumb', $image_uri);
  ?>
      <a href="<?php print file_create_url($node->field_img['und'][$i]['uri']); ?>" rel="group-<?php print $node->nid; ?>" class="colorbox">
        <img class="image<?php print ($i + 1);?>" src="<?php print $masthead_raw; ?>" />
      </a>
    <?php } ?>
</div>
