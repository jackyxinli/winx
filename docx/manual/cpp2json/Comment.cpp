#define TPL_USE_AUTO_ALLOC
#include "Comment.h"

// -------------------------------------------------------------------------

extern dom::Mark tagComment("comment");

impl::MarkedRule rComment(alloc, comment);

// -------------------------------------------------------------------------
