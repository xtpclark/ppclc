<?php

/**
 * @file
 * Theme settings for the theme866
 */
function theme866_form_system_theme_settings_alter(&$form, &$form_state) {
	/**
	 * Breadcrumb settings
	 */
	$form['theme866_breadcrumb'] = array(
		'#type' => 'fieldset',
		'#title' => t('Breadcrumb'),
	);
	
	$form['theme866_breadcrumb']['theme866_breadcrumb_display'] = array(
		'#type' => 'select',
		'#title' => t('Display breadcrumb'),
		'#default_value' => theme_get_setting('theme866_breadcrumb_display'),
		'#options' => array(
			'yes' => t('Yes'),
			'no' => t('No'),
		),
	);
	
	$form['theme866_breadcrumb']['theme866_breadcrumb_hideonlyfront'] = array(
		'#type' => 'checkbox',
		'#title' => t('Hide the breadcrumb if the breadcrumb only contains the link to the front page.'),
		'#default_value' => theme_get_setting('theme866_breadcrumb_hideonlyfront'),
	);
	
	$form['theme866_breadcrumb']['theme866_breadcrumb_showtitle'] = array(
		'#type' => 'checkbox',
		'#title' => t('Show page title on breadcrumb.'),
		'#default_value' => theme_get_setting('theme866_breadcrumb_showtitle'),
	);

	$form['theme866_breadcrumb']['theme866_breadcrumb_separator'] = array(
		'#type' => 'textfield',
		'#title' => t('Breadcrumb separator'),
		'#default_value' => theme_get_setting('theme866_breadcrumb_separator'),
	);
}