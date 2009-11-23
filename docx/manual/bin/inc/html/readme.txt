
== 2009-11-23 ==

php增加 esctext2html 函数，包括这些功能：

1. \p<eol> 或者 \n<eol> 转html之 <BR> 标签。其中<eol>为行末标记，即'\r' 或 '\n'。
2. \<Symbol> 转 <a href="...">Symbol</a>。其中 href 需要有link base才能转，这里面有些特别的东西需要做。相关内容可以见 @see 的处理代码。
3. html 标记的智能识别。即在文本中出现 <TAG 和 </TAG> 模式时，需要智能判断是否为 html 标记。如果不是，转成 &lt;TAG、&lt;/TAG&gt;，否则保留原串。

当TAG为这些时，认为是html标签：
"a", "b", "i", "u", "img", "p", "br", "pre", "code", 				
"h1", "h2", "h3", "h4",	"li", "ul", "ol", "font",
"table", "tr", "th", "td", "em"

== 2009-11-22 ==

function makerel($href, $file) -> $rel

该函数是求在 $file 文件中引用 $href 这个文件时，需要给出的链接($rel)。
或者说，是求 $href 相当于 $file 的一个相对路径。

例如，$href = "aaa/bbb/ccc/test.htm", $file = "aaa/ddd/e.htm"; 则 makerel($href, $file) 返回："../bbb/ccc/test.htm"。
