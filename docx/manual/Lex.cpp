
extern dom::NodeMark tagSentences("sentences", true);
extern dom::NodeMark tagVars("vars", true);
extern dom::NodeMark tagArgs("args", true);
extern dom::NodeMark tagItems("items", true);
extern dom::NodeMark tagCodedef("codedef");
extern dom::NodeMark tagTypedef("typedef");
extern dom::NodeMark tagServer("server");
extern dom::NodeMark tagFunction("function");
extern dom::NodeMark tagConstructor("ctor");
extern dom::NodeMark tagType("type");
extern dom::NodeMark tagNamedType("named_type");
extern dom::NodeMark tagCodedType("coded_type");
extern dom::NodeMark tagStruct("struct");
extern dom::NodeMark tagArray("array");

extern dom::Mark tagModule("module");
extern dom::Mark tagName("name");
extern dom::Mark tagValue("value");
extern dom::Mark tagSize("size");
extern dom::Mark tagId("id");
extern dom::Mark tagAsync("async");
extern dom::Mark tagCode("code");

impl::Allocator alloc;
impl::MarkedGrammar rType(alloc, sdl_type);
NS_STDEXT::String serverName;

dom::Document doc(alloc);
