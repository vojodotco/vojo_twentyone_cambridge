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

  $script->addLabel('begin');
  if ($name = voipnode_get_title($node)) {
    $script->addSay($name);
  } else {
    $script->addSay($node->title . '.');
  }

  // menu of group options
  $script->addLabel('group_menu');
  if ($greeting = voipnode_get_greeting($node)) {
    $script->addGetInput($greeting, 1, '', 6);
  } else {
    $script->addGetInput("^v('To hear other stories, press 1.  To record your own story press 2. To learn more press 3. If this is an emergency, press 4 to connect directly to help. To return and select another group press the pound key.',null,null,%vojo_voice)", 1, '', 6);
  }
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
  if ($descrip = voipnode_get_description($node)) {
    $script->addSay($descrip);  
  } else {
    $script->addSay($node->body . '.');
  }
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

