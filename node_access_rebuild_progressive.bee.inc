<?php
/**
 * @file
 * Defines the node access rebuild command.
 */

/**
 * Implements hook_bee_command().
 */
function node_access_rebuild_progressive_bee_command() {

  $items['node-access-rebuild-progressive'] = array(
    'description' => "Fully rebuild node access permissions.",
    'callback' => 'bee_node_access_rebuild_progressive_rebuild',
    'group' => 'content',
    'aliases' => array('narp'),
    'bootstrap' => BEE_BOOTSTRAP_FULL,
    'examples' => array(
      'bee narp' => bt('Rebuild all contact access permissions.'),
    ),
  );

  return $items;
}

/**
 * Rebuilds the node access grants table.
 */
function bee_node_access_rebuild_progressive_rebuild() {
  node_access_rebuild_progressive_trigger();
  $finished = FALSE;
  $total = db_query("SELECT count(nid) FROM {node}")->fetchField();
  $processed = 0;
  $finished = FALSE;
  while (!$finished) {
    $pass = node_access_rebuild_progressive_process_chunk();
    $processed += $pass['processed'];
    if (!empty($pass['total'])) {
      $figures = [
        '@pass' => $pass['processed'],
        '@nodes' => $pass['total'],
        '@processed' => $processed,
        '@total' => $total,
      ];
      bee_render_text(array('value' => bt('Processed @pass of @nodes nodes (@processed/@total).', $figures)));
    }
    else {
      $finished = TRUE;
    }
  }
  bee_render_text(array(
    'value' => bt('Finished rebuilding node access.'),
    '#color' => 'green',
  ));
}
