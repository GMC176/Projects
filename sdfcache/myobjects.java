import java.io.BufferedReader;
import java.io.FileReader;
import java.io.IOException;
import java.util.ArrayList;
import java.util.Iterator;
import java.time.LocalDateTime;
import java.time.format.DateTimeFormatter;
import java.io.RandomAccessFile;
import java.lang.Integer;
import java.lang.Math;
import java.lang.String;


class myobject {
    public LocalDateTime creationTime;
    String l;
    int pr=1;
    int num=0;
    int setno;
    public String getl(){
    return l;
    }
        
    
    public static void main(String[] args) {
       // System.out.println(args[0]+" "+args[1]+" "+args[2]);
        int p=Integer.parseInt(args[0]);
        int w=Integer.parseInt(args[1]);
        int bsize=Integer.parseInt(args[2]);
        int totalsets=(p*1024)/(w*64);
        
       System.out.println(totalsets);
        double setindex=(Math.log(totalsets))/(Math.log(2));
        int nobytesinsetindex=(int) setindex;
        String filePath = args[3]; 
        int miss=0,hit=0,total=0;
        ArrayList<Integer> noofelementsinset=new ArrayList<Integer>();
        ArrayList<Integer> missinset=new ArrayList<Integer>();
        ArrayList<Integer> hitinset=new ArrayList<Integer>();
        ArrayList<ArrayList<myobject>> myobjectlist = new ArrayList<>();
        for(int tt=0;tt<totalsets;tt++){
        missinset.add(0);
        }
        for(int q=0;q<totalsets;q++){
        hitinset.add(0);
        }
        for (int i = 0; i < (totalsets + 1000); i++) {
        ArrayList<myobject> row = new ArrayList<>();
   	myobjectlist.add(row);
	}

        byte[] buffer = new byte[8]; 
            int bytesread;
       // ArrayList<ArrayList<myobject>> myobjectlist = new ArrayList<>();
        

         try( RandomAccessFile file = new RandomAccessFile(filePath, "r")) {
            int k=0;
            
            while ((bytesread=file.read(buffer)) != -1) {
            if(bytesread!=1){
            int condition=0;
            //int k=0;
            String hexadecimal = new String(buffer,0,bytesread).trim();
            String line = Long.toBinaryString(Long.parseLong(hexadecimal, 16));
         //   int lengthofstring=line.length();
            line=String.format("%32s", line).replace(' ', '0');
       //     System.out.println(line);
         //   System.out.println(line.length());
            //System.out.println(binary);
            //String line = new String(buffer,0,bytesread).trim();
        //    System.out.println(Integer.parseInt(line.substring((26-nobytesinsetindex),26),2));
            int itssetno=Integer.parseInt(line.substring((26-nobytesinsetindex),26),2);
           // System.out.println(Integer.parseInt(line.substring((26-nobytesinsetindex),26),2)+"sss");
            ArrayList<myobject> target=new ArrayList<>();
            target=myobjectlist.get(itssetno);
            for(myobject test:target){
            if(line.compareTo(test.l)==0){
            condition=1;
            break;
            }}
            if(condition==0 && myobjectlist.get(itssetno).size()<w){
            myobject obj = new myobject();
            obj.l=line;
            obj.setno=itssetno;
            obj.num=k;
            k++;
           // System.out.println(k);
            myobjectlist.get(itssetno).add(obj);
            miss++;
            missinset.set(itssetno,(missinset.get(itssetno)+1));
            }
            else if(condition==0 && (myobjectlist.get(itssetno).size()==w)){
            int temp=100000000;
            int indexno=0;
            myobject test=new myobject();
            for(int j=0;j<w;j++){
            test=myobjectlist.get(itssetno).get(j);
            if(test.num<temp){
            indexno=j;
            temp=test.num;
            }}
            myobject obj = new myobject();
            obj.l=line;
            obj.setno=itssetno;
            obj.num=k;
            k++;
            missinset.set(itssetno,(missinset.get(itssetno)+1));
            //System.out.println(k);
            //myobjectlist.get(itssetno).set(indexno,obj);
          //  System.out.println("in set "+itssetno+" "+indexno+" " +line+" replaced "+(myobjectlist.get(itssetno).get(indexno)).l);
            myobjectlist.get(itssetno).set(indexno,obj);
            miss++;
            }
            if(condition==1){
            hit++;
            }
            if(condition==1 && myobjectlist.get(itssetno).size()<=w){
            for(myobject test:target){
            if(line.compareTo(test.l)==0){
            test.num=k;
            hitinset.set(itssetno,(hitinset.get(itssetno)+1));
            k++;
           // System.out.println("increased by one now it became" +k+ "from "+(k-1));
            }}}
           
            
            
            
            
            }}}
            catch(IOException e){
        System.out.println("s");
        } 
        int ss=0;    
           
        for(int a=0;a<totalsets;a++){
        if((myobjectlist.get(a).size())!=0){
        
        
        //System.out.println(myobjectlist.get(a).size());
        String printing="";
        for(myobject test:myobjectlist.get(a)){
        printing=printing+test.l+" ";
        ss=test.setno;
        }   
        System.out.println(printing+" misses are "+missinset.get(a)+" hits are "+hitinset.get(a)+" set number "+ss);
             }}
        System.out.println("total misses "+miss+" total hits "+hit);    
            }}
