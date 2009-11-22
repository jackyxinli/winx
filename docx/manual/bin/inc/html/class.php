<?php

function class_decl($fp, $template, $class)
{
	fwrite($fp, "<PRE class=\"syntax\"><B>");
	if (isset($template))
		fwrite($fp, htmlspecialchars($template->header) . "<BR>");
	fwrite($fp, "$class->keyword $class->name");
	if (isset($class->bases))
	{
		fwrite($fp, " : <BR>\n");
		foreach ($class->bases as $i => $base)
		{
			fwrite($fp, "&#x20;&#x20;");
			if ($i != 0)
				fwrite($fp, ",<BR>");
			if ($base->access)
				fwrite($fp, "$base->access ");
			fwrite($fp, $base->name);
		}
	}
	fwrite($fp, ";<BR/></B></PRE>\n");
}

function show_ctors($fp, $class, $env)
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
	
	return show_index($fp, $ctors, array_merge($env, array(
		"title" => "构造函数", "name" => "Constructor", "desc" => "Description")));
}

function show_methods($fp, $class, $env)
{
	show_fntable($fp, $class, array_merge($env, array(
		"fn" => "member", "title" => "方法列表", "name" => "Method", "desc" => "Description")));
}

function show_class($comment, $template, $class, $env)
{
	$file = $env["local"] . $class->name . ".htm";
	$fp = fopen($file, 'w');
	if (!$fp) {
		echo "---> ERROR: Create file `$file` failed!\n";
		return;
	}

	topic_start($fp, $comment, $class, $env);
	class_decl($fp, $template, $class);
	show_args($fp, $comment);
	show_desc($fp, $comment);
	show_remark($fp, $comment);
	{
		$env2 = array_merge($env, array(
			"base" => $env["base"] . "$class->name.",
			"local" => $env["local"] . "$class->name."));
		show_ctors($fp, $class, $env2);
		show_methods($fp, $class, $env2);
	}
	topic_end($fp, $comment, $env);
	
	fclose($fp);
}

?>
