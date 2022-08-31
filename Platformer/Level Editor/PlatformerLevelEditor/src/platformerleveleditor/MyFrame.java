/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package platformerleveleditor;

import java.awt.BorderLayout;
import java.awt.datatransfer.DataFlavor;
import java.awt.datatransfer.Transferable;
import java.awt.dnd.DnDConstants;
import java.awt.dnd.DropTarget;
import java.awt.dnd.DropTargetDragEvent;
import java.awt.dnd.DropTargetDropEvent;
import java.awt.dnd.DropTargetEvent;
import java.awt.dnd.DropTargetListener;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JMenu;
import javax.swing.JMenuBar;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.MouseEvent;
import java.awt.event.MouseListener;
import java.awt.event.MouseMotionListener;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.util.List;
import java.util.logging.Level;
import java.util.logging.Logger;
import javax.swing.JMenuItem;

/**
 *
 * @author Michael Jordan (14868336)
 */
public class MyFrame extends JFrame implements MouseListener, MouseMotionListener, DropTargetListener{


    public enum WindowMode {
        NULL, CREATE, SELECT, DELETE, LINK
    }
    
    public enum CreateMode {
        NULL, SPAWN, PLATFORM, BOX, BUTTON, DOOR, GUARD, TEXT, END
    }
    
    public int mousePosX, mousePosY;
    private WindowMode windowMode = WindowMode.NULL;
    private CreateMode createMode = CreateMode.NULL;
    
    private MyCanvas canvas = new MyCanvas();
    private JMenuBar menu = new JMenuBar();
    private JMenu file = new JMenu();
    
    private JLabel modeD = new JLabel();
    
    private JMenu edit = new JMenu();
    private JMenuItem clear = new JMenuItem();
    private JMenuItem Select = new JMenuItem();
    private JMenuItem Delete = new JMenuItem();
    
    private JMenu insert = new JMenu();
    private JMenu create = new JMenu(); 
   
    private JMenuItem platform = new JMenuItem();
    private JMenuItem spawn = new JMenuItem();
    private JMenuItem box = new JMenuItem();
    private JMenuItem button = new JMenuItem();
    private JMenuItem door = new JMenuItem();
    private JMenuItem guard = new JMenuItem();
    private JMenuItem text = new JMenuItem();
    private JMenuItem end = new JMenuItem();
    
    private JMenuItem newIni = new JMenuItem();
    private JMenuItem saveIni = new JMenuItem();
    private JMenuItem loadIni = new JMenuItem();
    
     private JMenuItem linker = new JMenuItem();
    
    private DropTarget t = new DropTarget(this, this);
    private boolean mousePressed, boundSelected;
    
    public static void main(String[] args){
        MyFrame frame = new MyFrame();
    }
    
    public MyFrame(){
        initialise();
        setVisible(true);
    }
    
    private void initialise()
    {
        setLayout(new BorderLayout());
        setSize(1280, 820);
        setTitle("Level Editior");
        modeD.setText("Mode: null");
        canvas.setSize(1280, 720);
        canvas.addMouseListener(this);
        canvas.addMouseMotionListener(this);
        add("North", modeD);
        add("Center", canvas);
        
        setJMenuBar(menu);
        
        file.setText("File");
        file.add(newIni);
        newIni.setText("Create new .ini");
        
        newIni.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent evt){clearIniFile();}
        });
        
        file.add(saveIni);
        saveIni.setText("Save As .ini");
        saveIni.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent evt){saveIniFile();}
        });
        
        file.add(loadIni);
        loadIni.setText("Load existing .ini");
        loadIni.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent evt){loadIniFile();}
        });
        
        menu.add(file);
        menu.add(edit);
        menu.add(insert);
        
        edit.setText("Edit");
        clear.setText("Clear Level");
        clear.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent evt){clearLevel();}
        });
        
        edit.add(clear);
        insert.setText("Insert");
        
        edit.add(Select);
        Select.setText("Select GameObject");
        Select.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent evt){selectMode();}
        });
        
        edit.add(Delete);
        Delete.setText("Delete GameObject");
        Delete.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent evt){deleteMode();}
        });
        
        insert.add(create);
        insert.add(linker);
        linker.setText("Link GameObjects");
        linker.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent evt){linkMode();}
        });
        
        create.setText("GameObjects:");
        
        
        create.add(spawn);
        spawn.setText("Spawn");
        
        spawn.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent evt){createAction(CreateMode.SPAWN);}
        });
        
        create.add(platform);
        platform.setText("Platform");
        
        platform.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent evt){createAction(CreateMode.PLATFORM);}
        });
        
        create.add(box);
        box.setText("Box");
        
        box.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent evt){createAction(CreateMode.BOX);}
        });
        
        create.add(button);
        button.setText("Button");
        button.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent evt){createAction(CreateMode.BUTTON);}
        });
        
        create.add(door);
        door.setText("Door");
        door.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent evt){createAction(CreateMode.DOOR);}
        });
        
        create.add(guard);
        guard.setText("Guard");
        guard.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent evt){createAction(CreateMode.GUARD);}
        });
        
        create.add(text);
        text.setText("Text");
        text.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent evt){createAction(CreateMode.TEXT);}
        });
        
        create.add(end);
        end.setText("End Location");
        end.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent evt){createAction(CreateMode.END);}
        });
        
        setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        setLocationRelativeTo(null);
    }
    
    private void createAction(CreateMode type){
        modeD.setText("Mode: Create GameObject: Player spawn.");
        windowMode = WindowMode.CREATE;
        createMode = type;
        switch(type)
        {
            case SPAWN:
                modeD.setText("Mode: Create GameObject: Player spawn.");
                break;
            case PLATFORM:
                modeD.setText("Mode: Create GameObject: Platform.");
                break;
            case BOX:
                modeD.setText("Mode: Create GameObject: Box.");
                break;
            case BUTTON:
                modeD.setText("Mode: Create GameObject: Button. (click on platform to add)");
                break;
            case DOOR:
                modeD.setText("Mode: Create GameObject: Door.");
                break;
            case GUARD:
                modeD.setText("Mode: Create GameObject: Guard Locations.");
                break;
            case END:
                modeD.setText("Mode: Create GameObject: End Location.");
                break;
            case TEXT:
                modeD.setText("Mode: Create GameObject: Background Text.");
                break;
        }
    }
        
    private void selectMode(){
        modeD.setText("Mode: Select GameObject.");
        windowMode = WindowMode.SELECT;
    }
        
    private void deleteMode()
    {
         modeD.setText("Mode: Delete GameObject.");
         windowMode = WindowMode.DELETE;
    }
    
    private void linkMode()
    {
        modeD.setText("Mode: Link GameObject.");
        windowMode = WindowMode.LINK;
    }
    
    private void clearLevel(){
        canvas.Clear();
        canvas.repaint();
        windowMode = WindowMode.NULL;
        modeD.setText("Mode: null");
    }
    
    private void clearIniFile(){
        canvas.Clear();
        IniParser.GetInstance().Close();
        canvas.repaint();
    }
    
    private void saveIniFile(){
        try {
            canvas.SaveToIni();
            IniParser.GetInstance().SaveFromFileChooser(this);
        } catch (IOException ex) {
            Logger.getLogger(MyFrame.class.getName()).log(Level.SEVERE, null, ex);
        }
    }
    
    private void loadIniFile(){
        
        try {
            IniParser.GetInstance().Clear();
            if(IniParser.GetInstance().LoadFromFileChooser(this))
            {
                canvas.Clear();
                LoadIniIntoCanvas();
            }
        } catch (FileNotFoundException ex) {
            Logger.getLogger(MyFrame.class.getName()).log(Level.SEVERE, null, ex);
        }
    }
    
    private void LoadIniIntoCanvas()
    {
        if(IniParser.GetInstance().DoesSectionExist("Platforms"))
            {
                int pLength = IniParser.GetInstance().GetValueAsInt("Platforms", "pCount");
                
                for(int i = 0; i < pLength; i++)
                {
                    canvas.AddPlatform(IniParser.GetInstance().GetValueAsString("Platforms", "p"+i));
                }
            }
            if(IniParser.GetInstance().DoesSectionExist("PlayerSpawn"))
            {
                canvas.AddSpawn(IniParser.GetInstance().GetValueAsString("PlayerSpawn", "spawn"));
            }
            if(IniParser.GetInstance().DoesSectionExist("EndPortal"))
            {
                canvas.AddEnd(IniParser.GetInstance().GetValueAsString("EndPortal", "end"));
            }
            if(IniParser.GetInstance().DoesSectionExist("Guards"))
            {
               int gLength = IniParser.GetInstance().GetValueAsInt("Guards", "gCount");
                
                for(int i = 0; i < gLength; i++)
                {
                    canvas.AddGuard(IniParser.GetInstance().GetValueAsString("Guards", "g"+i));
                }
            }
            if(IniParser.GetInstance().DoesSectionExist("Boxes"))
            {
                int BLength = IniParser.GetInstance().GetValueAsInt("Boxes", "BCount");
                
                for(int i = 0; i < BLength; i++)
                {
                    canvas.AddBox(IniParser.GetInstance().GetValueAsString("Boxes", "B"+i));
                }
            }
            
            if(IniParser.GetInstance().DoesSectionExist("Doors"))
            {
                int dLength = IniParser.GetInstance().GetValueAsInt("Doors", "dCount");
                
                for(int i = 0; i < dLength; i++)
                {
                    boolean state = IniParser.GetInstance().GetValueAsBool("Doors", "dOpen"+i);
                    canvas.AddDoor(IniParser.GetInstance().GetValueAsString("Doors", "dRect"+i), state);
                }
            }
            
            if(IniParser.GetInstance().DoesSectionExist("Buttons"))
            {
                int bLength = IniParser.GetInstance().GetValueAsInt("Buttons", "bCount");
                
                for(int i = 0; i < bLength; i++)
                {
                    canvas.AddButton(IniParser.GetInstance().GetValueAsString("Buttons", "b"+i));
                    if(IniParser.GetInstance().DoesSectionExist("b"+i))
                    {
                        int l = IniParser.GetInstance().GetValueAsInt("b"+i, "linkCount");
                        for(int j = 0; j < l; j++)
                        {
                            int d = IniParser.GetInstance().GetValueAsInt("b"+i, "L"+j);
                            canvas.LinkButtonToDoor(i, d);
                        }
                    }
                }
            }
            if(IniParser.GetInstance().DoesSectionExist("Text"))
            {
                int tLength = IniParser.GetInstance().GetValueAsInt("Text", "tCount");
                for(int i = 0; i < tLength; i++)
                {
                    String text = IniParser.GetInstance().GetValueAsString("Text", "tText"+i);
                    text = text.substring(1, text.length()-1);
                    int size = IniParser.GetInstance().GetValueAsInt("Text", "tSize"+i);
                    String loc = IniParser.GetInstance().GetValueAsString("Text", "tLoc"+i);
                    canvas.AddText(text, size, loc);
                }
            }
            canvas.repaint();
    }
    
    @Override
    public void mouseClicked(MouseEvent e) {
        switch(windowMode)
        {
            case CREATE:
                switch(createMode)
                {
                    case BOX:
                        if(canvas.HoldObjectAt(e.getX(), e.getY()))
                        {
                            canvas.DropHeldObject();
                        }
                        else
                        {
                            canvas.AddBox(e.getX() - 15, e.getY() - 15);
                        }
                        canvas.repaint();
                        break;
                    case BUTTON:
                        if(canvas.HoldObjectAt(e.getX(), e.getY()))
                        {
                            if(canvas.holdType == MyCanvas.HoldObjectType.PLATFORM)
                            {
                                canvas.AddButton(e.getX(), e.getY());
                            }
                        }
                        canvas.repaint();
                        break;
                    case GUARD:
                        canvas.AddGuard(e.getX(), e.getY());
                        canvas.repaint();
                        break;
                    case END:
                        canvas.MoveEndPortal(e.getX(), e.getY());
                        canvas.repaint();
                        break;
                    case TEXT:
                        int before = 0;
                        int after = 0;
                        if(IniParser.GetInstance().DoesSectionExist("Text"))
                        {
                            before = IniParser.GetInstance().GetValueAsInt("Text", "tCount");
                        }
                        TextInput t = new TextInput(this, "Text Input");
                        t.setLocation(e.getX(), e.getY());
                        t.pack();
                        t.setVisible(true);
                        if(IniParser.GetInstance().DoesSectionExist("Text"))
                        {
                            after = IniParser.GetInstance().GetValueAsInt("Text", "tCount");
                        }
                        if(after != before)
                        {
                            String text = IniParser.GetInstance().GetValueAsString("Text", "tText"+before);
                            text = text.substring(0, text.length()-1);
                            text = text.substring(1);
                            int size = IniParser.GetInstance().GetValueAsInt("Text", "tSize"+before);
                            canvas.AddText(text, size, e.getX(), e.getY());
                        }
                        canvas.repaint();
                        break;
                }
                break;
            case SELECT:
                break;
            case DELETE:
                canvas.RemoveAt(e.getX(), e.getY());
                canvas.repaint();
                break;
            default:
        }
    }

    @Override
    public void mousePressed(MouseEvent e) {
        
        
    }

    @Override
    public void mouseReleased(MouseEvent e) 
    {
        switch(windowMode)
            {
            case CREATE:
                switch(createMode)
                {
                    case PLATFORM:
                        canvas.AddPlatform(canvas.DisplayPlatform);
                        canvas.DisplayPlatform.x = 0;
                        canvas.DisplayPlatform.y = 0;
                        canvas.DisplayPlatform.w = 0;
                        canvas.DisplayPlatform.h = 0;
                        mousePressed = false;
                        break;
                    case SPAWN:
                        canvas.MovePlayerSpawn(e.getX(), e.getY());
                        break;
                    case DOOR:
                        canvas.AddDoor(canvas.DisplayPlatform);
                        canvas.DisplayPlatform.x = 0;
                        canvas.DisplayPlatform.y = 0;
                        canvas.DisplayPlatform.w = 0;
                        canvas.DisplayPlatform.h = 0;
                        mousePressed = false;
                        break;
                }
                canvas.repaint();
                break;
            case SELECT:
                if(boundSelected)
                    canvas.hold.DropBound();
                
                if(!canvas.HoldObjectAt(e.getX(), e.getY()))
                {
                    canvas.DropHeldObject();
                }
                canvas.repaint();
                mousePressed = false;
                break;
            case LINK:
                if(canvas.HoldObjectAt(e.getX(), e.getY()))
                {
                    canvas.LinkObjects();
                    canvas.ShowLine(false);
                    canvas.DropHeldObject();
                }
                canvas.repaint();
                mousePressed = false;
                break;
            default:
            }
    }

    @Override
    public void mouseEntered(MouseEvent e) {
        
    }
    @Override
    public void mouseExited(MouseEvent e) {
        
    }

    @Override
    public void mouseDragged(MouseEvent e) {
        switch(windowMode)
        {
        case CREATE:
            switch(createMode)
            {
                case PLATFORM:
                    if(mousePressed)
                    {
                        canvas.DisplayPlatform.w = e.getX() - canvas.DisplayPlatform.x;
                        canvas.DisplayPlatform.h = e.getY() - canvas.DisplayPlatform.y;
                    }
                    else
                    {
                        canvas.DisplayPlatform.x = e.getX();
                        canvas.DisplayPlatform.y = e.getY();
                        mousePressed = true;
                    }  
                break;
                case SPAWN:
                    canvas.MovePlayerSpawn(e.getX(), e.getY());
                    break;
                case DOOR:
                    if(mousePressed)
                    {
                        canvas.DisplayPlatform.w = e.getX() - canvas.DisplayPlatform.x;
                        canvas.DisplayPlatform.h = e.getY() - canvas.DisplayPlatform.y;
                    }
                    else
                    {
                        canvas.DisplayPlatform.x = e.getX();
                        canvas.DisplayPlatform.y = e.getY();
                        mousePressed = true;
                    } 
            }
            canvas.repaint();
            break;
        case SELECT:
            if(mousePressed)
            {
                canvas.MoveHeldObject(e.getX() - mousePosX, e.getY() - mousePosY);
                
                mousePosX = e.getX();
                mousePosY = e.getY();
            }
            else
            {
                if(canvas.HoldObjectAt(e.getX(), e.getY()))
                {
                    mousePressed = true;
                }
                mousePosX = e.getX();
                mousePosY = e.getY();
            }
            canvas.repaint();
            break;
        case LINK:
            if(mousePressed)
            {
                canvas.SetLinePointTwo(e.getX(), e.getY());
                
                mousePosX = e.getX();
                mousePosY = e.getY();
            }
            else
            {
                if(canvas.HoldObjectAt(e.getX(), e.getY()))
                {
                    if(canvas.holdType == MyCanvas.HoldObjectType.BUTTON || canvas.holdType == MyCanvas.HoldObjectType.DOOR)
                    {
                        canvas.SetLinePointOne(e.getX(), e.getY());
                        canvas.ShowLine(true);
                        mousePressed = true;
                    }
                    
                }
                mousePosX = e.getX();
                mousePosY = e.getY();
            }
            canvas.repaint();
            break;
        }
    }

    @Override
    public void mouseMoved(MouseEvent e) {
        
    }
    
    @Override
    public void dragEnter(DropTargetDragEvent dtde) {
        //Do Nothing
    }

    @Override
    public void dragOver(DropTargetDragEvent dtde) {
        //Do Nothing
    }

    @Override
    public void dropActionChanged(DropTargetDragEvent dtde) {
        //Do Nothing
    }

    @Override
    public void dragExit(DropTargetEvent dte) {
        //Do Nothing
    }

    @Override
    public void drop(DropTargetDropEvent event) {
        // Accept copy drops
        event.acceptDrop(DnDConstants.ACTION_COPY);

        // Get the transfer which can provide the dropped item data
        Transferable transferable = event.getTransferable();

        // Get the data formats of the dropped item
        DataFlavor[] flavors = transferable.getTransferDataFlavors();

        // Loop through the flavors
        for (DataFlavor flavor : flavors) {

            try {

                // If the drop items are files
                if (flavor.isFlavorJavaFileListType()) {

                    // Get all of the dropped files
                    List<File> files = (List) transferable.getTransferData(flavor);

                    // Loop them through
                    for (File file : files) 
                    {
                        String name = file.getName();
                        String ext = "";
                        int i = name.lastIndexOf('.');
                        if(i > 0 && i < name.length() - 1)
                        {
                            ext = name.substring(i+1).toLowerCase();
                        }
                        if(ext.equals("ini"))
                        {
                            IniParser.GetInstance().Clear();
                            IniParser.GetInstance().Load(file);
                            canvas.Clear();
                            LoadIniIntoCanvas();
                            break;
                        }
                    }

                }

            } catch (Exception e) {

                // Print out the error stack
                e.printStackTrace();

            }
        }
    }
}
