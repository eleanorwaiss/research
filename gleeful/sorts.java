class Sorts
{
  private static int findmax(Comparable [] list, int stop)
  {
    int maxpos=0;
    for(int i=1; i<stop; i++)
      if(list[i].compareTo(list[maxpos])>0)
        maxpos=i;
    return maxpos;
  }

  private static void swap(Object [] list, int x, int y)
  {
    Object temp=list[x];
    list[x]=list[y];
    list[y]=temp;
  }

  public static void selection(Comparable [] list)
  {
    for(int stop=list.length; stop>1; stop--)
    {
      int maxpos=findmax(list,stop);
      swap(list,maxpos,stop-1);
    }
  }

  public static void insertion(Comparable [] list)
  {
    for(int toinsert=1; toinsert<list.length; toinsert++)
    {
      Comparable save=list[toinsert];
      int j;
      for(j=toinsert-1; j>=0; j--)
      {
        if(list[j].compareTo(save)>0)
          list[j+1]=list[j];
        else
        {
          break;
        }
      }
      list[j+1]=save; 
    }
  }

  public static void bubble(Comparable [] list)
  {
    boolean done=false;
    while(!done)
    {
      done=true;
      for(int i=0; i+1<list.length; i++)
        if(list[i].compareTo(list[i+1])>0)
          { 
            swap(list,i,i+1); 
            done=false; 
          }
    }
  }
  /*
    for(int j=0; j<list.length; j++)
      for(int i=0; i+1<list.length; i++)
        if(list[i].compareTo(list[i+1])>0)
          swap(list,i,i+1); 
  */

  // Shell sort functions

  private static void insertion(Comparable [] list, int gap, int offset)
  //public static void insertion(Comparable [] list, int gap, int offset)
  {
    for(int toinsert=offset+gap; toinsert<list.length; toinsert+=gap)
    {
      Comparable save=list[toinsert];
      int j;
      for(j=toinsert-gap; j>=0; j-=gap)
      {
        if(list[j].compareTo(save)>0)
          list[j+gap]=list[j];
        else break;
      }
      list[j+gap]=save; 
    }
  }

  public static void shell(Comparable [] list)
  {
    int gap=list.length/5+1;
    while(gap>1)
    {
      for(int offset=0; offset<gap; offset++)
        insertion(list,gap,offset);
      gap=(int)(gap/2.2);
    }
    insertion(list); // gap=1, offset=0
  }

  // Quicksort functions

  private static int partition(Comparable [] list, int start, int stop)
  // returns the postion of the pivot
  {
    Comparable pivot=list[stop];
    int red=0; // number of items < pivot
    
    for(int frontier=start; frontier<stop; frontier++)
    {
      if(list[frontier].compareTo(pivot)<0)
      {
        swap(list,start+red,frontier);
        red++;
      }
    }
    swap(list,start+red,stop);

    for(int i=start; i<=stop; i++)
      System.out.print(list[i]+" ");
    System.out.println(" pivot="+list[start+red]);

    return start+red;
  }

  public static void quick(Comparable [] list)
  {
    quick(list,0,list.length-1);
  }

  private static void quick(Comparable [] list, int start, int stop)
  {
    // base cases
    // length 0 or 1
    if(stop<=start) return;
    // length 2 - for efficiency
    if(stop==start+1)
    {
      if(list[stop].compareTo(list[start])<0)
        swap(list,start,stop);
      return;
    }

    // recursive case
    int pivotpos=partition(list,start,stop);
    quick(list,start,pivotpos-1);
    quick(list,pivotpos+1,stop);
  }

  // merge sort functions

  public static void merge(Comparable [] list)
  {
    merge(list,0,list.length-1);
  }

  private static void merge(Comparable [] list, int start, int stop)
  {
    // base cases
    // length 0 or 1
    if(stop<=start) return;
    // length 2 - for efficiency
    if(stop==start+1)
    {
      if(list[stop].compareTo(list[start])<0)
        swap(list,start,stop);
      return;
    }

    // recursive case
    int mid=(stop+start)/2;
    merge(list,start,mid);
    merge(list,mid+1,stop);
    domerge(list,start,mid,stop);
  }

  private static void domerge(Comparable [] list, int start, int mid, int stop)
  {
    // create array to hold merged items
    Comparable [] ml=new Comparable[stop-start+1];
    int left=start, right=mid+1;
    for(int i=0; i<ml.length; i++)
    {
      // if item comes from the left
      if( left<=mid &&
          (right>stop ||  list[left].compareTo(list[right])<=0))
        ml[i]=list[left++];
      else
        ml[i]=list[right++];
    }

    for(int i=0; i<ml.length; i++)
      list[start+i]=ml[i];
  }

}




