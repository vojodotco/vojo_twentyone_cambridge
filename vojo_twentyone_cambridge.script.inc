<?php
/**
 * @file
 *     voipscripts for 21 Questions Cambridge group
 */
 
/**
 * Script to use when a group extension is selected
 *
 * $param['nid'] = Node ID.
 */
function _vojo_twentyone_cambridge_group_script($node) {
  $script = new VoipScript('vojo_twentyone_cambridge_group');

  $script->addSet('language', '');
  
  $script->addLabel('begin');
  if ($name = voipnode_get_title($node)) {
    $script->addSay($name);
  } else {
    $script->addSay($node->title . '.');
  }
  
  $script->addGotoIf('group_menu', "^%language != ''");
  $script->addGosub('vojo_twentyone_cambridge_pick_language');

  // menu of group options
  $script->addLabel('group_menu');
  $script->addGotoIf('group_menu_pt', "^%language == 'pt'");
  
  $script->addLabel('group_menu_en');
  if ($greeting = voipnode_get_greeting($node)) {
    $script->addGetInput($greeting, 1, '', 6);
  } else {
    $script->addGetInput("^v('To hear other stories, press 1.  To record your own story press 2. To learn more press 3. If this is an emergency, press 4 to connect directly to help. To return and select another group press the pound key.',null,null,%vojo_voice)", 1, '', 6);
  }
  $script->addGoto('handle_input');
  
  $script->addLabel('group_menu_pt');
  $script->addGetInput('http://vojo.co/sites/default/files/1351477370.wav', 1, '', 6);
  $script->addGoto('handle_input');
  
  $script->addLabel('handle_input');
  $script->addGotoIf('hear_stories', "^%input_digits == '1'");
  $script->addGotoIf('record_story', "^%input_digits == '2'");
  $script->addGotoIf('learn_more', "^%input_digits == '3'");
  $script->addGotoIf('get_help', "^%input_digits == '4'");
  $script->addGotoIf('return', "^%input_digits == '#'");
  $script->addSay("^v('Input not recognized',null,null,%vojo_voice)");
  $script->addGoto('group_menu');
  
  // user wants to hear other stories - this delegates to a voipview
  $script->addLabel('hear_stories');
  $params = array('view_name' => 'vojo_group_stories_voip', 'display_id' => 'vojo_group_stories_voip', 
    'args' => json_encode(array($node->nid)));
  $script->addGosub('voipviews_read_view', $params);
  $script->addGoto('begin');

  // user wants to record a story
  $script->addLabel('record_story');
  $script->addGosub('vojo_record_blog', array('nid' => $node->nid));
  $script->addGoto('begin');

  // user wants to hear more about the group
  $script->addLabel('learn_more');
  
  $script->addGotoIf('learn_more_pt', "^%language == 'pt'");
  
  $script->addLabel('learn_more_en');
  if ($descrip = voipnode_get_description($node)) {
    $script->addSay($descrip);  
  } else {
    $script->addSay($node->body . '.');
  }
  $script->addGoto('begin');

  $script->addLabel('learn_more_pt');
  $script->addGetInput('http://vojo.co/sites/default/files/1351477661.wav', 1, '', 6);
  $script->addGoto('begin');
  
  // user needs help right now
  $script->addLabel('get_help');
  $script->addSay(v('Redirecting'));
  // copies https://github.com/leoburd/pph/blob/master/pph.scripts.inc
  $help_number = variable_get(VOJO_TWENTYONE_CAMBRIDGE_HELP_NUMBER_VAR, '');
  $script->addDial($help_number);
  $script->addGoto('return');

  $script->addGosub('vojo_record_blog', array('nid' => $node->nid));
  $script->addGoto('begin');  

  // return
  $script->addLabel('return');
  $script->addReturn();
  return $script;
}

/**
 * Script to pick a language.  Based on _vojo_pick_language_script().
 */
function _vojo_twentyone_cambridge_pick_language_script(){

  $script = new VoipScript('vojo_twentyone_cambridge_pick_language');

  // pick language menu
  $script->addLabel('menu');
  $script->addGetInput("^v('For english, press one. Para português, pressione dois. To go back press pound.',null,null,%vojo_voice)", 1, '', 6);
  $script->addGotoIf('picked_en', "^%input_digits == '1'");
  $script->addGotoIf('picked_pt', "^%input_digits == '2'");
  $script->addGotoIf('return', "^%input_digits == '#'");
  $script->addSay("^v('Input not recognized',null,null,%vojo_voice)");
  $script->addGoto('menu');

  // user picked english
  $script->addLabel('picked_en');
  $script->addSet('language', 'en');
  $script->addGoSub('vojo_change_language', array('language'=>'en') );
  $script->addGoto('return');

  // user picked portuguese
  $script->addLabel('picked_pt');
  $script->addSet('language', 'pt');
  $script->addGoSub('vojo_change_language', array('language'=>'pt') );
  $script->addGoto('return');

  // return
  $script->addLabel('return');
  $script->addReturn();

  return $script;
  
}

