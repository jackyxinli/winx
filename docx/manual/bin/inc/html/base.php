<?php

// -------------------------------------------------------------------------
// tag or var sets
$trans_newline = array(
	"\\p" . PHP_EOL => "<br>",
	"\\n" . PHP_EOL => "<br>",
);

$html_tags = array(
	"a" 	=> true, "b" 	=> true, "i" 	=> true, "u" 	=> true,
	"img" 	=> true, "p" 	=> true, "br" 	=> true, "pre" 	=> true,
	"code" 	=> true, "h1" 	=> true, "h2" 	=> true, "h3" 	=> true,
	"h4" 	=> true, "li" 	=> true, "ul" 	=> true, "ol" 	=> true,
	"font" 	=> true, "table" => true, "tr" 	=> true, "th" 	=> true,
	"td" 	=> true, "em" 	=> true
);


// -------------------------------------------------------------------------
// utilities
function encode_angle_bracket($text)
{
	global $html_tags;
	$off = 0;
	$str = "";
	$len = strlen($text);
	$tag_states = array();

	for ($i = 0; $i < $len; $i++)
	{
		$cur = $text[$i];
		if ($cur ==  '<')
		{
			$j = $i + 1;
			while($text[$j] == ' ') $j ++;
			for ($j; $j < $len; $j ++)
			{
				$ch = $text[$j];
				if ($ch == ' ' || $ch == '>')
					break;
			}
			$tlen = $j - $i - 1;
			$tagname = substr($text, $i+1, $tlen);
			$key = ($tagname[0] == '/') ? strtolower(substr($tagname, 1)) : strtolower($tagname);
			if (@$html_tags[$key])
			{
				array_push($tag_states, "yes");
				$str .= "<$tagname";
			}
			else
			{
				array_push($tag_states, "no");
				$str .= "&lt;$tagname";
			}
			$i += $tlen;
		} // end of "if ($cur ==  '<')"
		else if ($cur == '>')
		{
			$val = array_pop($tag_states);
			if ($val == "yes")
				$str .= '>';
			else if (NULL == $val)
				$str .= '>';
			else 
				$str .= '&gt;';
		} // end of "else if ($cur == '>')"
		else
		{
			$str .= $cur;
		}
	}
	return $str;
}

function match_reference($text, $file, $env)
{
	$off = 0;
	$str = "";
	$total_len = strlen($text);
	
	for (;;)
	{
		$pos = strpos($text, '\\<', $off);
		if ($pos === false)
		{
			$str .= substr($text, $off);
			break;
		}
		$len  = $pos - $off;
		$str .= substr($text, $off, $len); 
		$off = $pos + 2;
		
		$pre_flag = false;
		for ($i = $off; $i < $total_len; $i++)
		{
			$ch = $text[$i];
			if ( $ch == '<' )
			{
				$pre_flag = true;
				break;
			}
			else if ($ch == '>')
			{
				break;
			}
		}
		$key = substr($text, $off, $i - $off);
		if ($pre_flag)
		{
			$str .= "\<" . $key;
			$off = $i;
		}
		else
		{
			$off = $i + 1;		
			$href = str_replace('::', '/', $key) . '.htm';
			$pos = strpos($href, '/');
			if ($pos === false)
				$href = $env['base'] . $href;
			else if ($pos === 0)
				$href = substr($href, 1);
			$href = makerel($href, $file);
			$str .= "<a href=\"$href\"><b>$key</b></a>";
		}
	}
	return $str;
}

function esctext2html($text, $file, $env)
{
	global $html_tags;
	global $trans_newline;
	$text = strtr($text, $trans_newline);
	$text = match_reference($text, $file, $env);
	return encode_angle_bracket($text);
}

function pathspec($name)
{
	return str_replace(array('::', '*', '/', '?'), array('_', 'MUL', 'DIV', '$'), $name);
}

function makerel($href, $file)
{
	$off = 0;
	for (;;)
	{
		$pos = strpos($href, '/', $off);
		if ($pos === false)
			break;
		$len = $pos + 1 - $off;
		if (substr($href, $off, $len) != substr($file, $off, $len))
			break;
		$off = $pos + 1;
	}
	
	$p = '';
	$href = substr($href, $off);
	for (;;)
	{
		$pos = strpos($file, '/', $off);
		if ($pos === false)
			return $p . $href;
		$p .= '../';
		$off = $pos + 1;
	}
}

function extended_text($fp, $rtf, $file, $env)
{
	foreach ($rtf as $item)
	{
		if (isset($item->text))
		{
			$text = esctext2html($item->text, $file, $env);
			fwrite($fp, "$text");
		}
		else if (isset($item->table))
		{
			fwrite($fp, 
"<TABLE height=\"1\"><TR VALIGN=\"top\">
<TH align=\"left\" width=\"35%\" height=\"19\">Value</TH>
<TH align=\"left\" width=\"65%\" height=\"19\">Meaning</TH>
</TR>\n");
			foreach ($item->table->vals as $v)
			{
				$text = esctext2html($v->text, $file, $env);
				fwrite($fp,
"<TR VALIGN=\"top\"><TD width=\"35%\" height=\"19\">$v->name</TD>
<TD width=\"65%\" height=\"19\">$text</TD></TR>\n");
			}
			fwrite($fp, "</TABLE>\n");
		}
	}
}

function show_args($fp, $comment, $file, $env)
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
		extended_text($fp, $arg->body, $file, $env);
		fwrite($fp, "</DD>");
	}
	fwrite($fp, "</DL>\n");
}

function show_sees($fp, $sees, $file, $env)
{
	if (!isset($sees))
		return;
	
	$notfirst = false;
	fwrite($fp, "<H4>See Also</H4><DL><DT><B>Reference</B></DT><DD>");
	foreach ($sees as $see)
	{
		foreach ($see->topics as $topic)
		{
			$href = str_replace('::', '/', $topic->name) . '.htm';
			$pos = strpos($href, '/');
			if ($pos === false)
				$href = $env['base'] . $href;
			else if ($pos === 0)
				$href = substr($href, 1);
			$href = makerel($href, $file);
			$text = (isset($topic->text) ? $topic->text : $topic->name);
			if ($notfirst)
				fwrite($fp, ' | ');
			else
				$notfirst = true;
			fwrite($fp, "<a href=$href><b>$text</b></a>");
		}
	}
	fwrite($fp, "</DD></DL>\n");
}

// -------------------------------------------------------------------------
// topic start/end

function html_header($fp, $title, $file, $env)
{
	$respath = makerel('res', $file);
	fwrite($fp,
"<HEAD>
<META http-equiv=\"Content-Type\" content=\"text/html; charset=utf-8\">
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

function show_brief($fp, $comment, $file, $env)
{
	if (!isset($comment))
		return;

	if (!isset($comment->brief))
	{
		if (isset($comment->summary))
			fwrite($fp, $comment->summary);
		return;
	}
	extended_text($fp, $comment->brief, $file, $env);
}

function show_desc($fp, $comment)
{
}

function show_retval($fp, $comment, $file, $env)
{
	if (!isset($comment) || !isset($comment->return))
		return;
	
	fwrite($fp, "<H4>返回值</H4>\n");
	extended_text($fp, $comment->return, $file, $env);
}

function show_remark($fp, $comment, $file, $env)
{
	if (!isset($comment) || !isset($comment->remark))
		return;
	
	fwrite($fp, "<H4>注意点</H4>\n");
	extended_text($fp, $comment->remark, $file, $env);
}

function topic_start($fp, $comment, $rel, $file, $env)
{
	fwrite($fp, "<HTML>");
	$category = @$env['category'];
	$header = $env['nsdisp'] . $rel;
	$title = isset($category) ? "$header - $category" : $header;
	html_header($fp, $title, $file, $env);
	fwrite($fp, "<BODY TOPMARGIN=\"0\">\n");
	if (isset($category))
		header_bar($fp, $category);
	fwrite($fp, "<H1>${header}</H1>");
	show_brief($fp, $comment, $file, $env);
}

function topic_end($fp, $comment, $file, $env)
{
	$respath = makerel('res', $file);
	//@@todo: example
	fwrite($fp, "<DIV CLASS=\"itfBorder\"><IMG SRC=\"$respath/tiny.gif\" width=\"1\" height=\"1\"/></DIV>");
	//@@todo: require
	show_sees($fp, @$comment->sees, $file, $env);
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

function macro_decl($fp, $macro)
{
	fwrite($fp, "<PRE class=\"syntax\"><B>#define $macro</B></PRE>\n");
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

	topic_start($fp, $comment, $rel, $file, $env);
	fn_decl($fp, @$s->template, $fn);
	show_args($fp, $comment, $file, $env);
	show_retval($fp, $comment, $file, $env);
	show_desc($fp, $comment);
	show_remark($fp, $comment, $file, $env);
	topic_end($fp, $comment, $file, $env);
	
	fclose($fp);
}

function show_global_fn($comment, $s, $env)
{
	$global = $s->global;
	$file = $env['base'] . $global->name . '.htm';
	$fp = fopen($file, 'w');
	if (!$fp) {
		echo "---> ERROR: Create file `$file` failed!\n";
		return;
	}
	
	//$env2 = array_merge($env, array('nsdisp' => '::'));
	topic_start($fp, $comment, $global->name, $file, $env);
	fn_decl($fp, @$s->template, $global);
	show_args($fp, $comment, $file, $env);
	show_retval($fp, $comment, $file, $env);
	show_desc($fp, $comment);
	show_remark($fp, $comment, $file, $env);
	topic_end($fp, $comment, $file, $env);
	
	fclose($fp);
}

function show_macro($comment, $sentence, $env)
{
	$macro_name = $sentence->macro->name;
	
	$args = $comment->args;
	$macro_args = '(';
	foreach ($args as $arg)
	{
		$macro_args = $macro_args . $arg->name;
	}
	$macro_args = $macro_args . ')';
	$macro_name = $macro_name . $macro_args;
	
	$file = $macro_name . '.htm';
	$fp = fopen($file, 'w');
	if (!$fp) {
		echo "---> ERROR: Create file `$file` failed!\n";
		return;
	}
	
	$env2 = array_merge($env, array('nsdisp' => 'macro '));
	topic_start($fp, $comment, $macro_name, $file, $env2);
	macro_decl($fp, $macro_name);
	show_args($fp, $comment, $file, $env);
	show_retval($fp, $comment, $file, $env);
	show_desc($fp, $comment);
	show_remark($fp, $comment, $file, $env);
	topic_end($fp, $comment, $file, $env);
	
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

function show_index($fp, $fns, $file, $env)
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
		$rel = pathspec($name) . ($overload ? "($args_text)" : "");
		$href = makerel("$base$rel.htm", $file);
		
		fwrite($fp, "<TR VALIGN=\"top\">
<TD width=\"35%\" height=\"19\"><b><a href=$href>$name($args_text)</a></b>\n</TD>
<TD width=\"65%\" height=\"19\">\n"); 
		show_brief($fp, $fns[$i], $file, $env);
		fwrite($fp, "</TD></TR>\n");
		show_fn($fns[$i], $fns[$i+1], $rel, $env);
	}
	fwrite($fp, "</TABLE>\n");
}

function show_fntable($fp, $code, $file, $env)
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
	show_index($fp, $fns, $file, $env);
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

function show_ctors($fp, $class, $file, $env)
{
	return show_fntable($fp, $class, $file, array_merge($env, array(
		'fntype' => "ctor", "title" => "构造函数", "name" => "Constructor", "desc" => "Description")));
}


function show_dtors($fp, $class, $file, $env)
{
	return show_fntable($fp, $class, $file, array_merge($env, array(
		'fntype' => "dtor", "title" => "析构函数", "name" => "Destructor", "desc" => "Description")));
}

function show_methods($fp, $class, $file, $env)
{
	show_fntable($fp, $class, $file, array_merge($env, array(
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

	topic_start($fp, $comment, $class->name, $file, array_merge($env, array('nsdisp' => 'class ')));
	class_decl($fp, $s->template, $class);
	show_args($fp, $comment, $file, $env);
	show_desc($fp, $comment);
	show_remark($fp, $comment, $file, $env);
	{
		$base = $env['base'] . "$class->name/";
		@mkdir($base);
		$env2 = array_merge($env, array(
			'base' => $base, 'nsdisp' => "$class->name::"));
		show_ctors($fp, $class, $file, $env2);
		show_dtors($fp, $class, $file, $env2); 
		show_methods($fp, $class, $file, $env2);
	}
	topic_end($fp, $comment, $file, $env);
	fclose($fp);
		
	foreach ($s->class->sentences as $s)
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
			
			if (isset($comment->topic))
			{
				$topic = $comment->topic;
				/*if ($topic->type == "macro")
				{
					show_macro($comment, $env);
				}*/
			}
		}
		else
		{
			if (!isset($base))
			{
				$base = './';
			}
			if (isset($s->macro))
			{
				show_macro($comment, $s, $env);
			}
			if (isset($s->class))
			{
				show_class($comment, $s, $env);
			}
			else if (isset($s->global))
			{
				show_global_fn($comment, $s, $env);
			}
			unset($comment);
		}
	}
			
			
			
		
}

// -------------------------------------------------------------------------

?>
