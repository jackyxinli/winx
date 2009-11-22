<?php
	$env = array(
		'ns' => '', 'base' => '', 'local' => 'default/',
		'respath' => '../res', 'path' => 'default/');
	
	foreach ($doc->sentences as $s)
	{
		if (isset($s->comment))
		{
			$comment = $s->comment;
			if (isset($comment->category))
				$env['category'] = $comment->category;
			if (isset($comment->ns)) {
				$env['local'] = $env['path'] = $comment->ns . '/';
			}
		}
		else
		{
			if (isset($s->class))
			{
				show_class($comment, $s, $env);
			}
			unset($comment);
		}
	}
?>
