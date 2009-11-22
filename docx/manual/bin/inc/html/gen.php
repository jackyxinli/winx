<?php
	$env = array(
		"respath" => "res", "titlefmt" => "%s", "headerfmt" => "%s",
		"base" => "", "local" => "output/");
	
	foreach ($doc->sentences as $s)
	{
		if (isset($s->comment))
		{
			$comment = $s->comment;
			if (isset($comment->category))
			{
				$env["category"] = $comment->category;
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
