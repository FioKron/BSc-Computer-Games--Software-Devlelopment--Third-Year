using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Data.Entity;

namespace CFNDS
{
    // Auto-generated code for a console application:
    class Program
    {


        static void Main(string[] args)
        {
            // Contains the posts for all of the blogs:
            BloggingContext BlogDatabaseContext = new BloggingContext();

            // 'Create and save a new Blog':
            Console.Write("Enter a name for a new Blog: ");
            string BlogName = Console.ReadLine();

            Blog DefaultBlog = new Blog { Name = BlogName };
            BlogDatabaseContext.BlogSet.Add(DefaultBlog);
            BlogDatabaseContext.SaveChanges();

            // 'Display all Blogs from the database':
            IOrderedQueryable<Blog> Query =
                from BlogTarget in BlogDatabaseContext.BlogSet
                orderby BlogTarget.Name
                select BlogTarget;

            Console.WriteLine("All Blogs in the database:");
            foreach (Blog Item in Query)
            {
                Console.WriteLine(Item.Name);
            }

            Console.WriteLine("Press any key to exit...");
            Console.ReadLine();
        }
    }

    // To access blogs and posts:
    public class BloggingContext : DbContext
    {
        public DbSet<Blog> BlogSet { get; set; }
        public DbSet<Post> PostSet { get; set; }
        public DbSet<User> Users { get; set; }

        protected override void OnModelCreating(DbModelBuilder modelBuilder)
        {
            modelBuilder.Entity<User>()
                .Property(User => User.DisplayName)
                .HasColumnName("Display_Name");
        }
    }
}
