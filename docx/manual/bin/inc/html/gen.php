<?php
	ob_start();

	$env = array(
		"respath" => "res", "titlefmt" => "%s", "headerfmt" => "%s",
		"base" => "");
	
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
				show_class($comment, $s->template, $s->class, $env);
				save("output/" . $s->class->name . ".htm");
			}
			unset($comment);
		}
	}
	
	ob_end_flush();
?>
