<?php

// -------------------------------------------------------------------------
// utilities

function make_href($name)
{
	return $name;
}

function extended_text($fp, $rtf)
{
	foreach ($rtf as $item)
	{
		if (isset($item->text))
			fwrite($fp, "$item->text");
		else if (isset($item->table))
		{
			fwrite($fp, 
"<TABLE height=\"1\"><TR VALIGN=\"top\">
<TH align=\"left\" width=\"35%\" height=\"19\">Value</TH>
<TH align=\"left\" width=\"65%\" height=\"19\">Meaning</TH>
</TR>\n");
			foreach ($item->table->vals as $v)
			{
				fwrite($fp,
"<TR VALIGN=\"top\"><TD width=\"35%\" height=\"19\">$v->name</TD>
<TD width=\"65%\" height=\"19\">$v->text</TD></TR>\n");
			}
			fwrite($fp, "</TABLE>\n");
		}
	}
}

function show_args($fp, $comment)
{
	if (!isset($comment))
		return;
	
	if (!isset($comment->args))
		return;
	
	fwrite($fp, "<H4>参数说明</H4><DL>");
	foreach ($comment->args as $arg)
	{
		fwrite($fp, "<DT>");
		if (isset($arg->attr))
			fwrite($fp, "[$arg->attr] ");
		fwrite($fp, "<I>$arg->name</I></DT><DD>");
		extended_text($fp, $arg->body);
		fwrite($fp, "</DD>");
	}
	fwrite($fp, "</DL>\n");
}

// -------------------------------------------------------------------------
// topic start/end

function res_path($base)
{
	$p = '';
	$off = 0;
	for (;;)
	{
		$pos = strpos($base, '/', $off);
		if ($pos === false)
			return $p . 'res';
		$p .= '../';
		$off = $pos + 1;
	}
}

function html_header($fp, $title, $env)
{
	$respath = res_path($env['base']);
	fwrite($fp,
"<HEAD>
<META http-equiv=\"Content-Type\" content=\"text/html; charset=gbk\">
<TITLE>$title</TITLE>
<SCRIPT SRC=\"$respath/langref.js\"></SCRIPT>
<LINK REL=\"STYLESHEET\" HREF=\"$respath/backsdk4.css\">
</HEAD>
");
}

function header_bar($fp, $category)
{
	fwrite($fp,
"<TABLE CLASS=\"buttonbarshade\" CELLSPACING=\"0\">
<TR><TD>&#x20;</TD></TR>
</TABLE>
<TABLE CLASS=\"buttonbartable\" CELLSPACING=\"0\">
<TR ID=\"hdr\">
<TD CLASS=\"runninghead\" NOWRAP=\"NOWRAP\">$category</TD>
</TR>
</TABLE>
");
}

function show_brief($fp, $comment)
{
	if (!isset($comment))
		return;
		
	if (!isset($comment->brief))
	{
		if (isset($comment->summary))
			fwrite($fp, $comment->summary);
		return;
	}
	extended_text($fp, $comment->brief);
}

function show_desc($fp, $comment)
{
}

function show_retval($fp, $comment)
{
	if (!isset($comment) || !isset($comment->return))
		return;
	
	fwrite($fp, "<H4>返回值</H4>\n");
	extended_text($fp, $comment->return);
}

function show_remark($fp, $comment)
{
	if (!isset($comment) || !isset($comment->remark))
		return;
	
	fwrite($fp, "<H4>注意点</H4>\n");
	extended_text($fp, $comment->remark);
}

function topic_start($fp, $comment, $rel, $env)
{
	fwrite($fp, "<HTML>");
	$category = $env['category'];
	$header = $env['nsdisp'] . $rel;
	$title = isset($category) ? "$header - $category" : $header;
	html_header($fp, $title, $env);
	fwrite($fp, "<BODY TOPMARGIN=\"0\">\n");
	if (isset($category))
		header_bar($fp, $category);
	fwrite($fp, "<H1>$header</H1>");
	show_brief($fp, $comment);
}

function topic_end($fp, $comment, $env)
{
	$respath = res_path($env['base']);
	//@example
	fwrite($fp, "<DIV CLASS=\"itfBorder\"><IMG SRC=\"$respath/tiny.gif\" width=\"1\" height=\"1\"/></DIV>");
	//@require
	//@see
	fwrite($fp, "</BODY></HTML>");
}

// -------------------------------------------------------------------------
// show fn

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

function fn_decl($fp, $template, $fn)
{
	fwrite($fp, "<PRE class=\"syntax\"><B>");
	if (isset($template))
		fwrite($fp, htmlspecialchars(trim($template->header)) . "<BR>");
	if (isset($fn->type))
		fwrite($fp, "$fn->type");
	if (isset($fn->calltype))
		fwrite($fp, "$fn->calltype ");
	fwrite($fp, "$fn->name(</B>");
	if (isset($fn->args))
	{
		foreach ($fn->args as $i => $arg)
		{
			if ($i != 0)
				fwrite($fp, ',');
			fwrite($fp, "<BR><B>&#x20;&#x20;$arg->type</B><I>$arg->name</I>");
			if (isset($arg->defval))
				fwrite($fp, " = $arg->defval");
		}
		fwrite($fp, "<BR>");
	}
	$fnattr = ($fn->funcattr ? " $fn->funcattr" : "");
	fwrite($fp, "<B>)$fnattr;<BR></B></PRE>\n");
}

function show_fn($comment, $s, $rel, $env)
{
	$fntype = $env['fntype'];
	$fn = $s->$fntype;
	$file = $env['base'] . $rel . '.htm';
	$fp = fopen($file, 'w');
	if (!$fp) {
		echo "---> ERROR: Create file `$file` failed!\n";
		return;
	}

	topic_start($fp, $comment, $rel, $env);
	fn_decl($fp, @$s->template, $fn);
	show_args($fp, $comment);
	show_retval($fp, $comment);
	show_desc($fp, $comment);
	show_remark($fp, $comment);
	topic_end($fp, $comment, $env);
	
	fclose($fp);
}

// -------------------------------------------------------------------------
// show fntable

function has_overload($fns, $fntype, $name, $i, $n)
{
	if ($i >= 2 && $fns[$i - 1]->$fntype->name == $name)
		return true;
	return ($i + 2 < $n && $fns[$i+3]->$fntype->name == $name);
}

function show_index($fp, $fns, $env)
{
	$count = count($fns);	
	if ($count == 0)
		return;
	
	fwrite($fp, "<H4>$env[title]</H4>
<TABLE height=\"1\">
<TR VALIGN=\"top\">
<TH align=\"left\" width=\"35%\" height=\"19\">$env[name]</TH>
<TH align=\"left\" width=\"65%\" height=\"19\">$env[desc]</TH>
</TR>");
	
	$base = $env['base'];
	$relpath = substr($base, strlen($base) - strpos(strrev($base), '/', 1));
	$fntype = $env['fntype'];
	
	for ($i = 0; $i < $count; $i += 2)
	{
		$fn = $fns[$i+1]->$fntype;
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
		$overload = has_overload($fns, $fntype, $name, $i, $count);
		$rel = make_href($name) . ($overload ? "($args_text)" : "");
		
		fwrite($fp, "<TR VALIGN=\"top\">
<TD width=\"35%\" height=\"19\"><b><a href=$relpath$rel.htm>$name($args_text)</a></b>\n</TD>
<TD width=\"65%\" height=\"19\">\n"); show_brief($fp, $fns[$i]);
		fwrite($fp, "</TD></TR>\n");
		
		show_fn($fns[$i], $fns[$i+1], $rel, $env);
	}
	fwrite($fp, "</TABLE>\n");
}

function show_fntable($fp, $code, $env)
{
	$fntype = $env['fntype'];
	foreach ($code->sentences as $s)
	{
		if (isset($s->comment))
			$comment = $s->comment;
		else
		{
			if (isset($s->$fntype) && isset($s->$fntype->funcattr))
			{
				$fns[] = $comment;
				$fns[] = $s;
			}
			unset($comment);
		}
	}
	show_index($fp, $fns, $env);
}

// -------------------------------------------------------------------------
// show class

function class_decl($fp, $template, $class)
{
	fwrite($fp, "<PRE class=\"syntax\"><B>");
	if (isset($template))
		fwrite($fp, htmlspecialchars(trim($template->header)) . "<BR>");
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
	return show_fntable($fp, $class, array_merge($env, array(
		'fntype' => "ctor", "title" => "构造函数", "name" => "Constructor", "desc" => "Description")));
}

function show_methods($fp, $class, $env)
{
	show_fntable($fp, $class, array_merge($env, array(
		'fntype' => "member", "title" => "方法列表", "name" => "Method", "desc" => "Description")));
}

function show_class($comment, $s, $env)
{
	$class = $s->class;
	$file = $env['base'] . $class->name . '.htm';
	$fp = fopen($file, 'w');
	if (!$fp) {
		echo "---> ERROR: Create file `$file` failed!\n";
		return;
	}

	topic_start($fp, $comment, $class->name, $env);
	class_decl($fp, $s->template, $class);
	show_args($fp, $comment);
	show_desc($fp, $comment);
	show_remark($fp, $comment);
	{
		$base = $env['base'] . "$class->name/";
		@mkdir($base);
		$env2 = array_merge($env, array(
			'base' => $base, 'nsdisp' => "$class->name::"));
		show_ctors($fp, $class, $env2);
		show_methods($fp, $class, $env2);
	}
	topic_end($fp, $comment, $env);
	
	fclose($fp);
}

// -------------------------------------------------------------------------

?>
