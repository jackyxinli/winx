<?php

function html_header($title) { echo
"<HEAD>
	<TITLE>$title</TITLE>
	<SCRIPT SRC=\"$RES_PATH/langref.js\"/>
	<LINK REL=\"STYLESHEET\" HREF=\"$RES_PATH/backsdk4.css\"/>
</HEAD>
"; }

function header_bar($category) { echo
"<TABLE CLASS=\"buttonbarshade\" CELLSPACING=\"0\">
	<TR>
		<TD>&#x20;</TD>
	</TR>
</TABLE>
<TABLE CLASS=\"buttonbartable\" CELLSPACING=\"0\">
	<TR ID=\"hdr\">
		<TD CLASS=\"runninghead\" NOWRAP=\"NOWRAP\">$category</TD>
	</TR>
</TABLE>
<H1>$title</H1>
"; }

function extended_text($rtf)
{
	echo "$rtf";
}

function topic_start($category, $title, $brief)
{
	header_bar($categroy);
	echo "<H1>$title</H1>";
	extended_text($brief);
}

function show_args($args)
{
	if (!isset($args))
		return;
		
	echo "<H4>参数说明</H4><DL>";
	for ($args as $arg)
	{
		echo "<DT>";
		if ($arg->attr)
			echo "[$arg->attr] ";
		echo "<I>$arg->name</I></DT><DD>";
		extended_text($arg->body);
		echo "</DD>";
	}
	echo "</DL>\n";
}

function show_brief($codedoc)
{
	if (!isset($codedoc))
		return;
		
	if (!isset($codedoc->brief))
		return;
	
	extended_text($codedoc->brief);
}

function check_topic_name($codedoc, $code)
{
	if (!isset($code->name) || !isset($codedoc) || !isset($codedoc->topic) )
		return true;
	
	for ($codedoc->topic->args as $arg)
	{
		if ($arg == $code->name)
			return true;
	}
	return false;
}

function check_arg_names($codedoc, $code)
{
	if (!isset($codedoc) || !isset($codedoc->args) )
		return true;

	if (!isset($code->args))
		return false;

	$args1 = $codedoc->args;
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

/*@@todo: 一致性检查在topic自身处完成，而不是在引用处完成!
		if (!check_topic_name($fndoc, $fn))
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
function show_index($code, $fns, $env, $base)
{
	$count = count($fns);
	if ($count == 0)
		return;
	
	echo "<H4>$env[title]</H4>
	<TABLE height=\"1\">
		<TR VALIGN=\"top\">
			<TH align=\"left\" width=\"35%\" height=\"19\">
				<xsl:value-of select=\"$env[name]\"/>
			</TH>
			<TH align=\"left\" width=\"65%\" height=\"19\">$env[desc]</TH>
		</TR>";
	
	for ($i = 0; $i < $count; $i += 2)
	{
		$fn = $fns[$i+1];
		$args_text = "";
		foreach ($fn->args as $j => $arg)
		{
			if ($j != 0)
				$args_text .= ",";
			$args_text .= $arg;
		}

		$name = (isset($fn->name) ? $fn->name : $code->name);
		$href =	$base . make_href($name) . ($count > 1 ? "($args_text).htm" : ".htm");
		
		echo "<TR VALIGN=\"top\"><TD width=\"35%\" height=\"19\"><b><a href=$href>$name($args_text)</a></b>\n";
		echo "</TD><TD width=\"65%\" height=\"19\">\n";	show_brief($fns[$i]);
		echo "</TD></TR>";
	}
	echo "</TABLE>\n";
}

function show_fntable($code, $env, $base)
{
	$fn = $env["fn"];
	foreach ($code->sentences as $s)
	{
		if (isset($s->comment))
			$fndoc = $s->comment;
		else
		{
			if (isset($s->$fn) && isset($s->$fn->funcattr))
			{
				$fns[] = $fndoc;
				$fns[] = $s->$fn;
			}
			unset($fndoc);
		}
	}
	show_index($code, $fns, $env, $base);
}

?>
