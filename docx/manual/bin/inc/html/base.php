<?php

function html_header($title, $env) { echo
"<HEAD>
<META http-equiv=\"Content-Type\" content=\"text/html; charset=gbk\">
<TITLE>$title</TITLE>
<SCRIPT SRC=\"$env[respath]/langref.js\"></SCRIPT>
<LINK REL=\"STYLESHEET\" HREF=\"$env[respath]/backsdk4.css\">
</HEAD>
"; }

function header_bar($category) { echo
"<TABLE CLASS=\"buttonbarshade\" CELLSPACING=\"0\">
<TR><TD>&#x20;</TD></TR>
</TABLE>
<TABLE CLASS=\"buttonbartable\" CELLSPACING=\"0\">
<TR ID=\"hdr\">
<TD CLASS=\"runninghead\" NOWRAP=\"NOWRAP\">$category</TD>
</TR>
</TABLE>
"; }

function extended_text($rtf)
{
	foreach ($rtf as $item)
	{
		if (isset($item->text))
			echo $item->text;
	};
}

function show_brief($comment)
{
	if (!isset($comment))
		return;
		
	if (!isset($comment->brief))
		return;
	
	extended_text($comment->brief);
}

function show_desc($comment)
{
}

function show_remark($comment)
{
	if (!isset($comment))
		return;
		
	if (!isset($comment->remark))
		return;

	extended_text($comment->remark);
}

function topic_start($comment, $code, $env)
{
	echo "<HTML>";
	$title = sprintf($env["titlefmt"], $code->name);
	$header = sprintf($env["headerfmt"], $code->name);
	html_header($title, $env);
	echo "<BODY TOPMARGIN=\"0\">\n";
	if (isset($env["categroy"]))
		header_bar($env["categroy"]);
	echo "<H1>$header</H1>";
	show_brief($comment);
}

function topic_end($comment, $env)
{
	//@example
	echo "<DIV CLASS=\"itfBorder\"><IMG SRC=\"$env[respath]/tiny.gif\" width=\"1\" height=\"1\"/></DIV>";
	//@require
	//@see
	echo "</BODY></HTML>";
}

function show_args($comment)
{
	if (!isset($comment))
		return;
	
	if (!isset($comment->args))
		return;
	
	echo "<H4>参数说明</H4><DL>";
	foreach ($comment->args as $arg)
	{
		echo "<DT>";
		if (isset($arg->attr))
			echo "[$arg->attr] ";
		echo "<I>$arg->name</I></DT><DD>";
		extended_text($arg->body);
		echo "</DD>";
	}
	echo "</DL>\n";
}

function check_topic_name($comment, $code)
{
	if (!isset($code->name) || !isset($comment) || !isset($comment->topic) )
		return true;
	
	foreach ($comment->topic->args as $arg)
	{
		if ($arg == $code->name)
			return true;
	}
	return false;
}

function check_arg_names($comment, $code)
{
	if (!isset($comment) || !isset($comment->args) )
		return true;

	if (!isset($code->args))
		return false;

	$args1 = $comment->args;
	$args2 = $code->args;
	
	$count = count($args1);
	if ($count != count($args2))
		return false;
	for ($i = 0; $i < $count; ++$i)
	{
		if ($args1[$i]->name != $args2[$i]->name)
			return false;
	}
	return true;
}

function make_href($name)
{
	return $name;
}

function has_overload($fns, $name, $i, $n)
{
	if ($i >= 2 && $fns[$i - 1]->name == $name)
		return true;
	return ($i + 2 < $n && $fns[$i+3]->name == $name);
}

/*@@todo: 一致性检查在topic自身处完成，而不是在引用处完成!
		if (!check_topic_name($comment, $fn))
		{
			fwrite(STDERR, "错误：类 $name 在注释文档中的名称与代码不匹配!\n");
			continue;
		}

		if (!check_arg_names($ctordoc, $ctor))
		{
			fwrite(STDERR, "错误：类 $name 的构造函数 $name($args_text) 在注释文档中的参数名名称与代码不匹配!\n");
			continue;
		}
*/
function show_index($fns, $env, $base)
{
	$count = count($fns);
	if ($count == 0)
		return;
	
	echo "<H4>$env[title]</H4>
<TABLE height=\"1\">
<TR VALIGN=\"top\">
<TH align=\"left\" width=\"35%\" height=\"19\">$env[name]</TH>
<TH align=\"left\" width=\"65%\" height=\"19\">$env[desc]</TH>
</TR>";
	
	for ($i = 0; $i < $count; $i += 2)
	{
		$fn = $fns[$i+1];
		$args_text = "";
		if (isset($fn->args))
		{
			foreach ($fn->args as $j => $arg)
			{
				if ($j != 0)
					$args_text .= ",";
				$args_text .= $arg->name;
			}
		}
		
		$name = $fn->name;
		$href =	$base . make_href($name) . (has_overload($fns, $name, $i, $count) ? "($args_text).htm" : ".htm");
		
		echo "<TR VALIGN=\"top\"><TD width=\"35%\" height=\"19\"><b><a href=$href>$name($args_text)</a></b>\n";
		echo "</TD><TD width=\"65%\" height=\"19\">\n";	show_brief($fns[$i]);
		echo "</TD></TR>\n";
	}
	echo "</TABLE>\n";
}

function show_fntable($code, $env, $base)
{
	$fn = $env["fn"];
	foreach ($code->sentences as $s)
	{
		if (isset($s->comment))
			$comment = $s->comment;
		else
		{
			if (isset($s->$fn) && isset($s->$fn->funcattr))
			{
				$fns[] = $comment;
				$fns[] = $s->$fn;
			}
			unset($comment);
		}
	}
	show_index($fns, $env, $base);
}

function save($file)
{
	$fp = fopen($file, 'w');
	fwrite($fp, ob_get_contents());
	fclose($fp);
	ob_clean();
}

?>
