<?php

/**
 * @file
 * Defines the node access rebuild command.
 */

/**
 * Implements hook_drush_command().
 */
function node_access_rebuild_progressive_drush_command() {

  $items['node-access-rebuild-progressive'] = array(
    'description' => "Fully rebuild node access.",
    'callback' => 'drush_node_access_rebuild_progressive_rebuild',
  );

  return $items;
}

/**
 * Rebuilds the node access grants table.
 */
function drush_node_access_rebuild_progressive_rebuild() {
  node_access_rebuild_progressive_trigger();
  $finished = FALSE;
  $total = db_query("SELECT count(nid) FROM {node}")->fetchField();
  $processed = 0;
  while (!$finished) {
    $cmd = '_drush_node_access_rebuild_progressive_process(' . $total . ',' . $processed . ');';
    $data = drush_invoke_process('@self', 'php-eval', [$cmd]);
    $processed = $data['context']['_drush_node_access_rebuild_progressive_processed'];
    $finished = drush_get_error() || !$data || (isset($data['context']['_drush_node_access_rebuild_progressive_finished']) && $data['context']['_drush_node_access_rebuild_progressive_finished'] === TRUE);
  }
}

/**
 * Processes a pass of nodes.
 *
 * @param int $total
 *   Number of nodes to process.
 * @param int $processed
 *   Number of nodes processed.
 */
function _drush_node_access_rebuild_progressive_process(int $total, int $processed) {
  $pass = node_access_rebuild_progressive_process_chunk();
  $processed += $pass['processed'];
  drush_set_option('_drush_node_access_rebuild_progressive_processed', $processed);
  $figures = [
    '@pass' => $pass['processed'],
    '@nodes' => $pass['total'],
    '@processed' => $processed,
    '@total' => $total,
  ];
  if (empty($pass['total'])) {
    drush_set_option('_drush_node_access_rebuild_progressive_finished', TRUE);
    return;
  }
  drush_print(dt('Processed @pass of @nodes nodes (@processed/@total).', $figures));
}
