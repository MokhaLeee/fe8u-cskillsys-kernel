# User process

The skills popup is ordered as a stack to simplify the process. We may initialize and then push the learned skill index to the stack during lvup/promotion, and let the popup setup process directly pop the stack.

I have to admit that I failed to find any elegant solution to let kernel know which skills has been learned to merge popup. One of the most troubling problems is finding a suitable time to initialize. So I decided to temporarily use a less general solution to at least meet the needs.
