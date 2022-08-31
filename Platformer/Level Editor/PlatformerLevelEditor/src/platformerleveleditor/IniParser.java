/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package platformerleveleditor;

import java.awt.Component;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileWriter;
import java.io.IOException;
import java.util.ArrayList;
import java.util.LinkedHashMap;
import java.util.Scanner;
import javax.swing.JFileChooser;
import javax.swing.filechooser.FileFilter;

/**
 *
 * @author Michael Jordan (14868336)
 */
public class IniParser {
    private static IniParser instance;
    private File file;
    private LinkedHashMap<String, String> contents;
    
    static IniParser GetInstance()
    {
        if(instance == null)
            instance = new IniParser();
        return instance;
    }
    
    private IniParser()
    {
        contents = new LinkedHashMap<String, String>();
    }
    
    public void Clear()
    {
        contents.clear();
    }
    
    public void Close()
    {
        file = null;
        contents.clear();
    }
    
    public boolean IsEmpty()
    {
        return contents.isEmpty();
    }
    
    public void AddValue(String section, String key, String value)
    {
        contents.put(section+" | "+key, value);
    }
    public void SetValue(String section, String key, String value)
    {
        contents.remove(section+" | "+key);
        contents.put(section+" | "+key, value);
    }
    
    public void Save() throws IOException
    {
        if(file == null)
        {
            return;
        }
        LinkedHashMap<String, ArrayList<String>> convert = new LinkedHashMap<String, ArrayList<String>>();
        
        //Convert to section -> key+value
        for(String s : contents.keySet())
        {
            int div = s.indexOf('|');
            String section = s.substring(0, div - 1);
            String key = s.substring(div + 2);
            if(!convert.containsKey(section))
            {
                convert.put(section, new ArrayList<String>());
            }
            convert.get(section).add(key+"="+contents.get(s));
        }
        
        File outFile = file;
        outFile.createNewFile();
        FileWriter writer = new FileWriter(outFile);
        for(String s : convert.keySet())
        {
            writer.write("["+s+"]\n");
            for(int i = 0; i < convert.get(s).size(); i++ )
            {
                writer.write(convert.get(s).get(i)+"\n");
            }
        }
        writer.close();
    }
    public void SaveFromFileChooser(Component c) throws FileNotFoundException, IOException
    {
        if(file == null){
            JFileChooser fc = new JFileChooser();
            fc.setFileFilter(new IniFilter());
            fc.showSaveDialog(c);
            file = new File(fc.getSelectedFile().getAbsolutePath()+".ini");
        }
        Save();
    }
    public void Load(String fileLoc) throws FileNotFoundException
    {
        Load(new File(fileLoc));
    }
    
    public void Load(File inFile) throws FileNotFoundException
    {
        file = inFile;
        Scanner scan = new Scanner(inFile);
        String section = "";
        while(scan.hasNextLine())
        {
            String data = scan.nextLine();
            if(data.length() == 0 || data.charAt(0) == ';')
            {
                //Do nothing
            }
            else if(data.charAt(0) == '[')
            {
                section = data.substring(1, data.length()-1);
            }
            else if(section != "")
            {
                int loc = data.indexOf('=');
                String key = section+ " | "+ data.substring(0, loc);
                String value = data.substring(loc + 1);
                contents.put(key, value);
            }
        }
        scan.close();
    }
    
    public boolean LoadFromFileChooser(Component c) throws FileNotFoundException
    {
        JFileChooser fc = new JFileChooser();
        fc.setFileFilter(new IniFilter());
        int val = fc.showOpenDialog(c);
        if(val == JFileChooser.APPROVE_OPTION)
        {
            File file = fc.getSelectedFile();
            if(file.exists())
            {
                Load(file);
                return true;
            }
            return false;
        }
        return false;
    }
    public String GetValueAsString(String section, String key){
        return contents.get(section+" | "+key);
    }
    
    public boolean GetValueAsBool(String section, String key){
        String val = contents.get(section+" | "+key);
        if(val.toLowerCase().equals("true"))
        {
            return true;
        }
        else if (val.toLowerCase().equals("false"))
        {
            return false;
        }
        return false;
    }
    
    public int GetValueAsInt(String section, String key){
        return Integer.parseInt(contents.get(section+" | "+key));
    }
    
    public float GetValueAsFloat(String section, String key){
        return Float.parseFloat(contents.get(section+" | "+key));
    }
    
    public boolean DoesSectionExist(String section)
    {
        for(String s : contents.keySet())
        {
             if(s.contains(section))
             {
                 return true;
             }
        }
        return false;
    }
    
    private class IniFilter extends FileFilter{

        @Override
        public boolean accept(File f) {
            if(f.isDirectory()){
                return true;
            }
            
            String name = f.getName();
            String ext = "";
            int i = name.lastIndexOf('.');
            if(i > 0 && i < name.length() - 1)
            {
                ext = name.substring(i+1).toLowerCase();
            }
            return ext.equals("ini");
        }

        @Override
        public String getDescription() {
            return " .ini";
        }
    }
}

