<?php

/**
 * @file
 * Line chart displaying signature activity.
 */

// Plugin definition.
$plugin = array(
  'single' => TRUE,
  'title' => t('Revenues summary registration'),
  'description' => t('Displays a line chart showing events registration history and revenues amount.'),
  'category' => t('CRM Core Event'),
  'edit form' => 'revenues_sammary_registration_form',
  'render callback' => 'revenues_sammary_registration_render',
  'admin info' => 'revenues_sammary_registration_admin_info',
  'defaults' => array(
    'text' => '',
  ),
  'all contexts' => TRUE,
);

/**
 * Administration info.
 */
function revenues_sammary_registration_admin_info($subtype, $conf, $contexts) {
  $block = new stdClass();
  $block->title = t('Revenues summary registration');
  $block->content = t('Displays a line chart showing events registration history and revenues amount.');

  return $block;
}

/**
 * Settings form.
 */
function revenues_sammary_registration_form($form, &$form_state) {

  // Stored widget configuration.
  $conf = $form_state['conf'];

  return $form;
}

/**
 * Submit handler for settings form.
 */
function revenues_sammary_registration_form_submit($form, &$form_state) {
}

/**
 * Render callback.
 */
function revenues_sammary_registration_render($subtype, $conf, $panel_args, $context = NULL) {

  $content = '';

  if (is_numeric($panel_args[0])) {

    // Run the query.
    $data = db_query("
  SELECT COUNT( DISTINCT registration.activity_id) registrations,
  SUM(era.field_cmcev_amount_value) as revenue,
  ere.field_cmcev_event_target_id as event_nid,
  ed.field_cmcev_date_value as event_start_date,
  UNIX_TIMESTAMP(erd.field_cmcev_reg_date_value) AS ts,
  DATE_FORMAT(erd.field_cmcev_reg_date_value, '%b %e') AS rdate,
  DATE_FORMAT(erd.field_cmcev_reg_date_value, '%j') AS doy
  FROM {crm_core_activity} AS registration
  LEFT JOIN {field_data_field_cmcev_reg_date} AS erd ON erd.entity_id = registration.activity_id
  LEFT JOIN {field_data_field_cmcev_amount} AS era ON era.entity_id = registration.activity_id
  LEFT JOIN {field_data_field_cmcev_event} AS ere ON ere.entity_id = registration.activity_id
  LEFT JOIN {field_data_field_cmcev_date} AS ed ON ed.entity_id = ere.field_cmcev_event_target_id
  WHERE registration.type = 'cmcev_event_registration'
  AND erd.field_cmcev_reg_date_value IS NOT NULL
  AND erd.field_cmcev_reg_date_value <> ''
  AND ( (DATE_FORMAT(ADDTIME(ed.field_cmcev_date_value2, SEC_TO_TIME(10800)), '%Y%m') >= :yearmonth
AND DATE_FORMAT(ADDTIME(ed.field_cmcev_date_value, SEC_TO_TIME(10800)), '%Y%m') <= :yearmonth) )
  GROUP BY rdate, doy
  ORDER BY ts, rdate, doy DESC;
	", array(':yearmonth' => $panel_args[0]));

    // Normalize the query for zero values, avoid big arcs.
    $reg_item = array();
    $reg_series_label = array();

    $revenue_item = array();
    $revenue_series_label = array();

    $last_date = 0;
    $last_date_ts = 0;

    foreach ($data as $item) {
      // Check if today is greater than lastdate.
      if (($item->doy - $last_date) > 1 && $last_date != 0) {
        // Insert some zero values for yesterday and the day after lastdate.
        $reg_item[] = array($last_date_ts * 1000 + 86400 * 1000, 0);
        $reg_series_label[] = '---';
        $revenue_item[] = array($last_date_ts * 1000 + 86400 * 1000, 0);
        $revenue_series_label[] = '---';
        if (($item->doy - $last_date) > 2) {
          $reg_item[] = array($last_date_ts * 1000 + 86400 * 1000 * 2, 0);
          $reg_series_label[] = '---';
          $revenue_item[] = array($last_date_ts * 1000 + 86400 * 1000 * 2, 0);
          $revenue_series_label[] = '---';
        }
      }

      // Add the date.
      $reg_item[] = array($item->ts * 1000, $item->registrations);
      $revenue_item[] = array(
        $item->ts * 1000,
        isset($item->revenue) ? $item->revenue : 0,
      );

      // Record a label.
      $reg_series_label[] = format_plural($item->registrations, '@reg registration', '@reg registrations', array('@reg' => $item->registrations));
      $revenue_series_label[] = '$' . check_plain($item->revenue);

      // Set lastdate.
      $last_date = $item->doy;
      $last_date_ts = $item->ts;
    }

    // Draw the chart.
    $chart_data[] = new flotData($reg_item);

    $options = new flotStyleLine();
    $options->series->lines = new stdClass();
    $options->series->lines->show = TRUE;
    $options->xaxis = new stdClass();
    $options->xaxis->mode = "time";
    $options->xaxis->ticks = 7;
    $options->yaxis = new stdClass();
    $options->yaxis->min = 0;
    $options->series->seriesLabels = $reg_series_label;

    $options->seriesLabels = $reg_series_label;

    $content = theme('flot_graph', array(
      'data' => $chart_data,
      'options' => $options,
    ));
  }

  $block = new stdClass();

  $block->title = t('Registration');
  $block->content = $content;

  return $block;
}
