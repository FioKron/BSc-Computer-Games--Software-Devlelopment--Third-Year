using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

// Project namespace:
namespace CFNDS
{
    // For any posts to one's blog:
    public class Post
    {
        public int PostId { get; set; }
        public string Title { get; set; }
        public string Content { get; set; }

        public int BlogID { get; set; }

        // As Posts belong to a Blog:
        // (virtual, so that this navigation
        // property is lazily loaded)
        public virtual Blog PostOwner { get; set; }
    }
}
