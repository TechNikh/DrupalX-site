<?php

/**
 * @file
 * Administration page callbacks for the edx customizable search module.
 */

/**
 * customizable_search_cache_settings function
 * returns form object for General Settings in customizable search module.
 * @return $form
 */
function multisite_ui_settings_form($form, &$form_state) {
  $form = array();
  $form['multisite_ui_db'] = array(
    '#type' => 'fieldset',
    '#title' => t('Database'),
    '#description' => t('Configure the database settings.'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  $form['multisite_ui_db']['multisite_ui_db_host'] = array(
    '#title' => t('DB Host'), 
    '#type' => 'textfield', 
    '#required' => TRUE, 
    '#description' => t('database host. like 127.0.0.1'), 
    '#default_value' => variable_get('multisite_ui_db_host', '127.0.0.1'),
  );
  $form['multisite_ui_db']['multisite_ui_db_port'] = array(
    '#title' => t('DB Port number'), 
    '#type' => 'textfield', 
    '#required' => TRUE, 
    '#description' => t('database port. like 3306'), 
    '#default_value' => variable_get('multisite_ui_db_port', '3306'),
  );
  $form['multisite_ui_db']['multisite_ui_db_user'] = array(
    '#title' => t('DB User name'), 
    '#type' => 'textfield', 
    '#required' => TRUE, 
    '#description' => t('database user name. like root'), 
    '#default_value' => variable_get('multisite_ui_db_user', 'root'),
  );
  $form['multisite_ui_db']['multisite_ui_db_prefix'] = array(
    '#title' => t('DB Prefix'), 
    '#type' => 'textfield', 
    '#required' => TRUE, 
    '#description' => t('Prefix for the database name of the site.'), 
    '#default_value' => variable_get('multisite_ui_db_prefix', ''),
  );
  $form['multisite_ui_drush_path'] = array(
    '#title' => t('Drush bin directory'), 
    '#type' => 'textfield', 
    '#required' => TRUE, 
    '#description' => t('Path to Drush'), 
    '#default_value' => variable_get('multisite_ui_drush_path', ''),
  );
  $form['multisite_ui_drupal8_path'] = array(
    '#title' => t('Drupal8 root directory'), 
    '#type' => 'textfield', 
    '#required' => FALSE, 
    '#description' => t('Path to Drupal 8 root'), 
    '#default_value' => variable_get('multisite_ui_drupal8_path', ''),
  );
  return system_settings_form($form);
}

function multisite_ui_settings_form_submit($form, &$form_state) {
  if($form_state['values']['clear_memcache']){
    cache_clear_all('edx_search:first_page_cards_json', 'cache');
    cache_clear_all('edx_search:all_courses_indexed_json', 'cache');
    cache_clear_all('edx_search:all_courses_json', 'cache');
    cache_clear_all('edx_search:all_facets_json', 'cache');
    cache_clear_all('edx_search:empty_facets_json', 'cache');
    cache_clear_all('edx_search:all_search_json', 'cache');
    cache_clear_all('v', 'cache_customizable_search', TRUE); // Deleting all wildcard entries starting with "v"
    drupal_set_message("Cleared search object cache in memcache");
  }
  if($form_state['values']['clear_varnish']){
    $paths = array(
      'search/api/all' => 'search/api/all',
      'courses' => 'courses',
      'customizable_search/bundle/course' => 'customizable_search/bundle/course',
      'customizable_search/bundle/school' => 'customizable_search/bundle/school',
      'customizable_search/bundle/subject' => 'customizable_search/bundle/subject',
    );
    acquia_purge_expire_cache($paths);
    drupal_set_message("Cleared Varnish cache for search sections");
  }
}
