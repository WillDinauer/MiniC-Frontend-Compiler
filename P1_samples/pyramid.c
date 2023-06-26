extern void print (int);
 
void pyramid (int rows) 
{
   int i;
   int j;

   i = 1;
   j = 1;

   while (i <= rows)
   {
      while (j <= i)
      {
         print(j);
         j = j + 1;
      }
      i = i + 1;
   }
   return;
}
