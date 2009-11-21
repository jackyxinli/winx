<?php

function class_decl($template, $class)
{
	echo "<PRE class=\"syntax\"><B>";
	if (isset($template))
		echo htmlspecialchars($template->header) . "<BR>";
	echo "$class->keyword $class->name";
	if (isset($class->bases))
	{
		echo " : <BR>\n";
		foreach ($class->bases as $i => $base)
		{
			echo "&#x20;&#x20;";
			if ($i != 0)
				echo ",<BR>";
			if ($base->access)
				echo "$base->access ";
			echo $base->name;
		}
	}
	echo ";<BR/></B></PRE>\n";
}

function show_ctors($class, $base)
{
	foreach ($class->sentences as $s)
	{
		if (isset($s->comment))
			$comment = $s->comment;
		else
		{
			if (isset($s->ctor))
			{
				$ctors[] = $comment;
				$ctors[] = $s->ctor;
			}
			unset($comment);
		}
	}
	
	return show_index($ctors, array(
		"title" => "构造函数", "name" => "Constructor", "desc" => "Description"),
		$base . "$class->name");
}

function show_methods($class, $base)
{
	show_fntable($class, array(
		"fn" => "member", "title" => "方法列表", "name" => "Method", "desc" => "Description"),
		$base . "$class->name");
}

function show_class($comment, $template, $class, $env)
{
	topic_start($comment, $class, $env);
	class_decl($template, $class);
	show_args($comment);
	show_desc($comment);
	show_remark($comment);
	show_ctors($class, $env["base"]);
	show_methods($class, $env["base"]);
	topic_end($comment, $env);
}

?>
