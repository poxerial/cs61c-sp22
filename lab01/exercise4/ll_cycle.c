#include "ll_cycle.h"
#include <stddef.h>

int ll_has_cycle(node *head) {
  if (!head)
    return 0;
  node *fast_ptr = head;
  node *slow_ptr = head;
  do
  {
    if (fast_ptr->next == NULL || fast_ptr->next->next == NULL)        
      return 0;
    fast_ptr = fast_ptr->next->next;
    slow_ptr = slow_ptr->next;
  } while (fast_ptr != slow_ptr);
  return 1; 
}
