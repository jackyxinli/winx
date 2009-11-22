<?php
	$env = array('base' => 'default/', 'nsdisp' => '');
	
	foreach ($doc->sentences as $s)
	{
		if (isset($s->comment))
		{
			$comment = $s->comment;
			if (isset($comment->category))
				$env['category'] = $comment->category;
			if (isset($comment->ns))
			{
				$base = str_replace(array('::', '.', '\\'), array('/', '/', '/'), $comment->ns) . '/';
				$base = str_replace('//', '/', $base);
				@mkdir($base, 0700, true);
				$env['base'] = $base;
				$env['nsdisp'] = str_replace('/', '::', $base);
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
