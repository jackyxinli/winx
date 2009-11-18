
extern dom::NodeMark tagSentences("sentences", true);
extern dom::NodeMark tagBaseClasses("bases", true);
extern dom::NodeMark tagArgs("args", true);

extern dom::NodeMark tagTemplate("template");
extern dom::NodeMark tagClass("class");
extern dom::NodeMark tagMember("member");
extern dom::NodeMark tagConstructor("ctor");
extern dom::NodeMark tagTypeCast("typecast");
extern dom::NodeMark tagTypedef("type");
extern dom::NodeMark tagEnum("enum");

extern dom::Mark tagClassKeyword("keyword");
extern dom::Mark tagHeader("header");
extern dom::Mark tagAccess("access");
extern dom::Mark tagType("type");
extern dom::Mark tagCallType("calltype");
extern dom::Mark tagName("name");
extern dom::Mark tagAttr2("funcattr");
extern dom::Mark tagDefVal("defval");

impl::Allocator alloc;
impl::MarkedGrammar rCppSymbol(alloc, cppsymbol);
impl::MarkedGrammar rType(alloc, type);
String className;

dom::Document doc(alloc);
