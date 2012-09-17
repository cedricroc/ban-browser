<?php

/**
 * @file
 * Administrative page callbacks for the ban browser module.
 */

include_once dirname(__FILE__) . '/ban_browser.functions.inc';

/**
 * Implements hook_admin_settings() for module settings configuration.
 */
function ban_browser_admin_settings_form($form) {

  $result = db_select('ban_browser_infos', 'ban')
            ->fields('ban')
            ->execute();

  while($record = $result->fetchAssoc()) {

    $key = _hook_php_camelize($record['browser']);

    $form[$key] = array(
      '#type' => 'fieldset',
      '#title' => t($record['browser']),
      '#collapsible' => true,
      '#collapsed' => true,
    );


    $form[$key][$key . '_minVersion'] = array(
      '#title' => t('Minimum version allowed'),
      '#type' => 'textfield',
      '#default_value' => $record['minVersion'],
      '#description' => t('Leave empty to set no minimum version'),
    );


    $form[$key][$key . '_maxVersion'] = array(
      '#title' => t('Maximum version allowed'),
      '#type' => 'textfield',
      '#default_value' => $record['maxVersion'],
      '#description' => t('Leave empty to set no maximum version'),
    );
  }

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );

  return $form;
}

/**
 * Implements _form_validate().
 */
function ban_browser_admin_settings_form_validate($form, &$form_state) {

  $result = db_select('ban_browser_infos', 'ban')
            ->fields('ban')
            ->execute();

  while($record = $result->fetchAssoc()) {

    $key = _hook_php_camelize($record['browser']);

    if (!empty($form_state['values'][$key . '_minVersion']))
      $form_state['values'][$key . '_minVersion'] = floatval(trim($form_state['values'][$key . '_minVersion']));
    else
      $form_state['values'][$key . '_minVersion'] = null;



    if (!empty($form_state['values'][$key . '_maxVersion']))
      $form_state['values'][$key . '_maxVersion'] = floatval(trim($form_state['values'][$key . '_maxVersion']));
    else
      $form_state['values'][$key . '_maxVersion'] = null;
  }
}


/**
 * Form submission handler for ban_browser_admin_settings_form().
 *
 * @see ban_browser_admin_settings_form_validate()
 */
function ban_browser_admin_settings_form_submit($form, &$form_state) {

  $result = db_select('ban_browser_infos', 'ban')
            ->fields('ban')
            ->execute();

  while($record = $result->fetchAssoc()) {

    $key = _hook_php_camelize($record['browser']);

    $datas = array();
    $datas['minVersion'] = $form_state['values'][$key . '_minVersion'];
    $datas['maxVersion'] = $form_state['values'][$key . '_maxVersion'];

    db_update('ban_browser_infos')
      ->fields($datas)
      ->condition('id', $record['id'], '=')
      ->execute();
  }

  drupal_set_message(t('Your changes are been saved.'));
}