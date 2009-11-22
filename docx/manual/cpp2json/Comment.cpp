#define TPL_USE_AUTO_ALLOC
#include "Comment.h"

// -------------------------------------------------------------------------

extern dom::Mark tagTopicArgs("args", true);
extern dom::Mark tagTopicBrief("brief", true);
extern dom::Mark tagSeeTopics("topics", true);

extern dom::Mark tagCategory("category");
extern dom::Mark tagNamespace("ns");
extern dom::Mark tagTopicType("type");
extern dom::Mark tagText("text");
extern dom::Mark tagUnknown("unknown");
extern dom::Mark tagSummary("summary");
extern dom::Mark tagCaption("caption");
extern dom::Mark tagAttr("attr");

extern dom::NodeMark tagVals("vals", true);
extern dom::NodeMark tagDescs("descriptions", true);
extern dom::NodeMark tagSees("sees", true);

extern dom::NodeMark tagCommentDoc("comment");
extern dom::NodeMark tagTopic("topic");
extern dom::NodeMark tagBrief("brief", true); // extend text
extern dom::NodeMark tagTable("table");
extern dom::NodeMark tagBody("body", true); // extend text
extern dom::NodeMark tagReturn("return", true); // extend text
extern dom::NodeMark tagRemark("remark", true); // extend text

impl::MarkedRule rComment(alloc, comment);

// -------------------------------------------------------------------------
