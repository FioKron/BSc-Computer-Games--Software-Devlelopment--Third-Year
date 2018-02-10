using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

// Project namespace:
namespace CFNDS
{
    // A blog contains serveral posts:
    public class Blog
    {
        public int BlogID { get; set; }
        public string Name { get; set; }
        public string URL { get; set; }

        // As a blog consists of many Posts
        // (virtual, so that this navigation
        // property is lazily loaded):
        public virtual List<Post> PostList { get; set; }
    }
}
