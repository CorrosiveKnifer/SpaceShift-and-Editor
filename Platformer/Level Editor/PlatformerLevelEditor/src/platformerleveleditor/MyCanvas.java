/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package platformerleveleditor;

import java.awt.Canvas;
import java.awt.Color;
import java.awt.Graphics;

import java.util.ArrayList;
import javafx.scene.text.Font;
import platformerleveleditor.Drawable;
import platformerleveleditor.GameObjects.Box;
import platformerleveleditor.GameObjects.ButtonObject;
import platformerleveleditor.GameObjects.Door;
import platformerleveleditor.GameObjects.EndPortal;
import platformerleveleditor.GameObjects.Guard;
import platformerleveleditor.GameObjects.Platform;
import platformerleveleditor.GameObjects.PlayerSpawn;
import platformerleveleditor.GameObjects.TextObject;

/**
 *
 * @author Michael Jordan (14868336)
 */
class MyCanvas extends Canvas {
    
    enum HoldObjectType{
        NULL, SPAWN, PLATFORM, BOX, BUTTON, GUARD, DOOR, TEXT, END
    }
    
    private PlayerSpawn spawn;
    private ArrayList<Platform> platforms = new ArrayList<Platform>();
    private ArrayList<Box> boxes = new ArrayList<Box>();
    private ArrayList<ButtonObject> buttons = new ArrayList<ButtonObject>();
    private ArrayList<Door> doors = new ArrayList<Door>();
    private ArrayList<Guard> guards = new ArrayList<Guard>();
    private ArrayList<TextObject> texts = new ArrayList<TextObject>();
    private EndPortal end;
    public Platform DisplayPlatform = new Platform(0, 0, 0, 0);
    
    private int line1X, line1Y, line2X, line2Y;
    private boolean isShowingLine;
    public Drawable hold;
    public HoldObjectType holdType;
    
    public MyCanvas()
    {
        this.setBackground(Color.black);
    }
    
    @Override
    public void paint(Graphics g)
    {
        DisplayPlatform.Draw(g);
        if(hold != null)
        {
            hold.SetBoundDisplay(true);
        }
        for(int i = 0; i < texts.size(); i++){
            texts.get(i).Draw(g);
        }
        for(int i = 0; i < platforms.size(); i++){
            platforms.get(i).Draw(g);
        }
        for(int i = 0; i < guards.size(); i++){
            guards.get(i).Draw(g);
        }
        for(int i = 0; i < doors.size(); i++){
            doors.get(i).Draw(g);
        }
        for(int i = 0; i < boxes.size(); i++){
            boxes.get(i).Draw(g);
        }
        if(spawn != null)
            spawn.Draw(g);
        if(end != null)
            end.Draw(g);
        if(hold != null)
        {
            g.setColor(new Color(0.8f, 0.8f, 0.8f, 0.8f));
            g.fillRect( hold.x, hold.y - g.getFont().getSize(), 100, g.getFont().getSize() * 4);
            g.setColor(Color.black);
            g.setFont(new java.awt.Font("SansSerif", java.awt.Font.PLAIN , 12));
            g.drawString("Held Object", hold.x, hold.y);
            g.drawString("ID: "+hold.id, hold.x, hold.y + g.getFont().getSize());
            g.drawString("x: " + hold.x + ", y: " + hold.y, hold.x, hold.y + 2 * g.getFont().getSize());
            g.drawString("w: " + hold.w + ", h: " + hold.h, hold.x, hold.y + 3 * g.getFont().getSize());
        }
        
        if(isShowingLine)
        {
            g.setColor(Color.red);
            g.drawLine(line1X, line1Y, line2X, line2Y);
        }
        
    }
    
    public boolean HoldObjectAt(int _x, int _y)
    {
        for(int i = 0; i < buttons.size(); i++){
            if(buttons.get(i).IsOverlapping(_x, _y))
            {
                if(hold != null)
                {
                    hold.SetBoundDisplay(false);
                }
                hold = buttons.get(i);
                holdType = HoldObjectType.BUTTON;
                return true;
            }
        }
        for(int i = 0; i < texts.size(); i++){
            if(texts.get(i).IsOverlapping(_x, _y))
            {
                if(hold != null)
                {
                    hold.SetBoundDisplay(false);
                }
                hold = texts.get(i);
                holdType = HoldObjectType.TEXT;
                return true;
            }
        }
        for(int i = 0; i < platforms.size(); i++){
            if(platforms.get(i).IsOverlapping(_x, _y))
            {
                if(hold != null)
                {
                    hold.SetBoundDisplay(false);
                }
                hold = platforms.get(i);
                holdType = HoldObjectType.PLATFORM;
                return true;
            }
        }
        for(int i = 0; i < guards.size(); i++){
            if(guards.get(i).IsOverlapping(_x, _y))
            {
                if(hold != null)
                {
                    hold.SetBoundDisplay(false);
                }
                hold = guards.get(i);
                holdType = HoldObjectType.GUARD;
                return true;
            }
        }
        for(int i = 0; i < doors.size(); i++){
            if(doors.get(i).IsOverlapping(_x, _y))
            {
                if(hold != null)
                {
                    hold.SetBoundDisplay(false);
                }
                hold = doors.get(i);
                holdType = HoldObjectType.DOOR;
                return true;
            }
        }
        for(int i = 0; i < boxes.size(); i++){
            if(boxes.get(i).IsOverlapping(_x, _y))
            {
                if(hold != null)
                {
                    hold.SetBoundDisplay(false);
                }
                hold = boxes.get(i);
                holdType = HoldObjectType.BOX;
                return true;
            }
        }
        if(spawn != null)
        {
            if(spawn.IsOverlapping(_x, _y))
            {
                if(hold != null)
                {
                    hold.SetBoundDisplay(false);
                }
                hold = spawn;
                holdType = HoldObjectType.SPAWN;
                return true;
            }
        }
        if(end != null)
        {
            if(end.IsOverlapping(_x, _y))
            {
                if(hold != null)
                {
                    hold.SetBoundDisplay(false);
                }
                hold = end;
                holdType = HoldObjectType.END;
                return true;
            }
        }
        if(hold != null)
        {
            hold.SetBoundDisplay(false);
            holdType = HoldObjectType.NULL;
            hold = null;
        }
        return false;
    }
    
    public void LinkObjects()
    {
        int _x[] = {line1X, line2X};
        int _y[] = {line1Y, line2Y};
        int a = -1;
        int b = -1;
        for(int x = 0; x < 2; x++){
            for(int i = 0; i < buttons.size(); i++){
                if(buttons.get(i).IsOverlapping(_x[x], _y[x]))
                {
                    a = i;
                    break;
                }
            }
            for(int i = 0; i < doors.size(); i++){
                if(doors.get(i).IsOverlapping(_x[x], _y[x]))
                {
                    b = i;
                }
            }
        } 
        if(a != -1 && b != -1)
        {
            buttons.get(a).LinkTo(doors.get(b));
        }
    }
    
    public void MoveHeldObject(int _x, int _y)
    {
        if(hold != null)
        {
            hold.MoveHeld(_x, _y);
        }
    }
    
    public void DropHeldObject()
    {
        if(hold != null)
        {
            hold.SetBoundDisplay(false);
            hold = null;
        }
    }
    
    public void AddBox(int _x, int _y)
    {
        boxes.add(new Box(_x, _y));
    }
    
    public void AddButton(int _x, int _y)
    {
        for(int i = 0; i < platforms.size(); i++){
            if(platforms.get(i).IsOverlapping(_x, _y))
            {
                ButtonObject b = new ButtonObject(_x, _y);
                b.linkedTo = platforms.get(i);
                b.id = buttons.size();
                buttons.add(b);
                platforms.get(i).AddButton(b);
                break;
            }
        }
    }
    
    public void AddButtonTo(int _x, int _y, int _p)
    {
        ButtonObject b = new ButtonObject(_x, _y);
        b.linkedTo = platforms.get(_p);
        b.id = buttons.size();
        buttons.add(b);
        platforms.get(_p).AddButton(b);
    }
    
    public void SetLinePointOne(int _x, int _y)
    {
        line1X = _x;
        line1Y = _y;
    }
    
    public void SetLinePointTwo(int _x, int _y)
    {
        line2X = _x;
        line2Y = _y;
    }
    
    public void ShowLine(boolean _line)
    {
        isShowingLine = _line;
    }
    
    public void AddPlatform(int _x, int _y, int _w, int _h)
    {
        Platform p = new Platform(_x, _y, _w, _h);
        p.id =  platforms.size();
        platforms.add(p);
    }
    
    public void AddPlatform(Platform _rect)
    {
        Platform p = new Platform(_rect.x, _rect.y, _rect.w, _rect.h);
        p.id =  platforms.size();
        platforms.add(p);
    }
    
    public void AddDoor(Platform _rect)
    {
        Door d = new Door(_rect.x, _rect.y, _rect.w, _rect.h);
        d.id = doors.size();
        d.SetOpen(false);
        doors.add(d);
    }
    public void AddDoor(int _x, int _y, int _w, int _h, boolean IsOpen)
    {
        Door d = new Door(_x, _y, _w, _h);
        d.id = doors.size();
        d.SetOpen(IsOpen);
        doors.add(d);
    }
    
    public void LinkButtonToDoor(int buttonID, int doorID)
    {
        buttons.get(buttonID).LinkTo(doors.get(doorID));
    }
    
    public void MovePlayerSpawn(int _x, int _y)
    {
        if(spawn == null)
            spawn = new PlayerSpawn(_x, _y);
        spawn.x = _x;
        spawn.y = _y;
    }
    
    public void MoveEndPortal(int _x, int _y)
    {
        if(end == null)
            end = new EndPortal(_x, _y);
        end.x = _x;
        end.y = _y;
    }
    
    public void AddGuard(int _x, int _y)
    {
        Guard g = new Guard(_x, _y);
        g.id = guards.size();
        guards.add(g);
    }
    
    public void AddGuard(String _rect)
    {
        int x, y, w, h;
        x = Integer.parseInt(_rect.substring(0, _rect.indexOf('x')));
        y = Integer.parseInt(_rect.substring(_rect.indexOf('x') + 1, _rect.indexOf('y')));
        AddGuard(x, y);
    }
    
    public void AddBox(String _rect)
    {
        int x, y, w, h;
        x = Integer.parseInt(_rect.substring(0, _rect.indexOf('x')));
        y = Integer.parseInt(_rect.substring(_rect.indexOf('x') + 1, _rect.indexOf('y')));
        AddBox(x, y);
    }
    
    public void AddDoor(String _rect, boolean IsOpen)
    {
        int x, y, w, h;
        x = Integer.parseInt(_rect.substring(0, _rect.indexOf('x')));
        y = Integer.parseInt(_rect.substring(_rect.indexOf('x') + 1, _rect.indexOf('y')));
        w = Integer.parseInt(_rect.substring(_rect.indexOf('y') + 1, _rect.indexOf('w')));
        h = Integer.parseInt(_rect.substring(_rect.indexOf('w') + 1, _rect.indexOf('h')));
        AddDoor(x, y, w, h, IsOpen);
    }
    
    public void AddPlatform(String _rect)
    {
        int x, y, w, h;
        x = Integer.parseInt(_rect.substring(0, _rect.indexOf('x')));
        y = Integer.parseInt(_rect.substring(_rect.indexOf('x') + 1, _rect.indexOf('y')));
        w = Integer.parseInt(_rect.substring(_rect.indexOf('y') + 1, _rect.indexOf('w')));
        h = Integer.parseInt(_rect.substring(_rect.indexOf('w') + 1, _rect.indexOf('h')));
        AddPlatform(x, y, w, h);
    }
    
    public void AddButton(String _button)
    {
        int x, y, p;
        p = Integer.parseInt(_button.substring(0, _button.indexOf('p')));
        x = Integer.parseInt(_button.substring(_button.indexOf('p') + 1, _button.indexOf('x')));
        y = Integer.parseInt(_button.substring(_button.indexOf('x') + 1, _button.indexOf('y')));
        AddButtonTo(x, y, p);
    }
    
    public void AddSpawn(String _spawn)
    {
        int x, y;
        x = Integer.parseInt(_spawn.substring(0, _spawn.indexOf('x')));
        y = Integer.parseInt(_spawn.substring(_spawn.indexOf('x') + 1, _spawn.indexOf('y')));
        
        spawn = new PlayerSpawn(x, y);
        spawn.id = 0;
    }
    
    public void AddEnd(String _spawn)
    {
        int x, y;
        x = Integer.parseInt(_spawn.substring(0, _spawn.indexOf('x')));
        y = Integer.parseInt(_spawn.substring(_spawn.indexOf('x') + 1, _spawn.indexOf('y')));
        
        end = new EndPortal(x, y);
        end.id = 0;
    }
    
    public void AddText(String txt, int size, String loc)
    {
         int x, y;
        x = Integer.parseInt(loc.substring(0, loc.indexOf('x')));
        y = Integer.parseInt(loc.substring(loc.indexOf('x') + 1, loc.indexOf('y')));
        
        AddText(txt, size, x, y);
    }
    
    public void AddText(String txt, int size, int _x, int _y)
    {
        TextObject t = new TextObject(txt, size, _x, _y);
        t.id = texts.size();
        texts.add(t);
    }
    
    public void Clear()
    {
        platforms.clear();
        guards.clear();
        buttons.clear();
        doors.clear();
        boxes.clear();
        texts.clear();
        spawn = null;
        end = null;
    }
    
    public void SaveToIni()
    {
        IniParser.GetInstance().Clear();
        IniParser.GetInstance().AddValue("Platforms", "pCount", String.valueOf(platforms.size()));
        for(int i = 0; i < platforms.size(); i++)
        {
            IniParser.GetInstance().AddValue("Platforms", "p"+platforms.get(i).id, platforms.get(i).toString());
        }
        IniParser.GetInstance().AddValue("PlayerSpawn", "spawn", spawn.toString());
        IniParser.GetInstance().AddValue("EndPortal", "end", end.toString());
        IniParser.GetInstance().AddValue("Guards", "gCount", String.valueOf(guards.size()));
        for(int i = 0; i < guards.size(); i++)
        {
            IniParser.GetInstance().AddValue("Guards", "g"+guards.get(i).id, guards.get(i).toString());
        }  
        IniParser.GetInstance().AddValue("Boxes", "BCount", String.valueOf(boxes.size()));
        for(int i = 0; i < boxes.size(); i++)
        {
            IniParser.GetInstance().AddValue("Boxes", "B"+boxes.get(i).id, boxes.get(i).toString());
        }
        
        IniParser.GetInstance().AddValue("Doors", "dCount", String.valueOf(doors.size()));
        for(int i = 0; i < doors.size(); i++)
        {
            IniParser.GetInstance().AddValue("Doors", "dRect"+doors.get(i).id, doors.get(i).toString());
            IniParser.GetInstance().AddValue("Doors", "dOpen"+doors.get(i).id, String.valueOf(doors.get(i).GetOpen()));
        }
        
        IniParser.GetInstance().AddValue("Buttons", "bCount", String.valueOf(buttons.size()));
        for(int i = 0; i < buttons.size(); i++)
        {
            IniParser.GetInstance().AddValue("Buttons", "b"+buttons.get(i).id, buttons.get(i).toString());
            IniParser.GetInstance().AddValue("b"+buttons.get(i).id, "linkCount", String.valueOf(buttons.get(i).GetLinkSize()));
            for(int j = 0; j < buttons.get(i).GetLinkSize(); j++)
            {
                IniParser.GetInstance().AddValue("b"+buttons.get(i).id, "L"+j, String.valueOf(buttons.get(i).GetIDLinkAt(j)));
            }
        }
        IniParser.GetInstance().AddValue("Text", "tCount", String.valueOf(texts.size()));
        for(int i = 0; i < texts.size(); i++)
        {
            IniParser.GetInstance().AddValue("Text", "tText"+i, "\""+texts.get(i).GetText()+"\"");
            IniParser.GetInstance().AddValue("Text", "tSize"+i, String.valueOf(texts.get(i).GetFontSize()));
            IniParser.GetInstance().AddValue("Text", "tLoc"+i, texts.get(i).toString());
        }
    }
    
    public void UpdatePlatform(ArrayList<Platform> array)
    {
        for(int i = 0; i < array.size(); i++){
            array.get(i).id = i;
        }
    }
    
    public void UpdateBox(ArrayList<Box> array)
    {
        for(int i = 0; i < array.size(); i++){
            array.get(i).id = i;
        }
    }
    public void UpdateGuards(ArrayList<Guard> array)
    {
        for(int i = 0; i < array.size(); i++){
            array.get(i).id = i;
        }
    }
    public void UpdateButtons(ArrayList<ButtonObject> array)
    {
        for(int i = 0; i < array.size(); i++){
            array.get(i).id = i;
        }
    }
    public void UpdateDoors(ArrayList<Door> array)
    {
        for(int i = 0; i < array.size(); i++){
            array.get(i).id = i;
        }
    }
    public void UpdateTexts(ArrayList<TextObject> array)
    {
        for(int i = 0; i < array.size(); i++){
            array.get(i).id = i;
        }
    }
    public void RemoveAt(int _x, int _y)
    {
        for(int i = 0; i < platforms.size(); i++){
            if(platforms.get(i).IsOverlapping(_x, _y))
            {
                while(platforms.get(i).GetButtonCount() > 0)
                {
                    ButtonObject b = platforms.get(i).GetButtonAt(platforms.get(i).GetButtonCount() - 1);
                    platforms.get(i).RemoveButtonAt(platforms.get(i).GetButtonCount() - 1);
                    buttons.remove(b);
                }
                platforms.remove(i);
                UpdatePlatform(platforms);
            }
        }
        for(int i = 0; i < boxes.size(); i++){
            if(boxes.get(i).IsOverlapping(_x, _y))
            {
                boxes.remove(i);
                UpdateBox(boxes);
            }
        }
        for(int i = 0; i < guards.size(); i++){
            if(guards.get(i).IsOverlapping(_x, _y))
            {
                guards.remove(i);
                UpdateGuards(guards);
            }
        }
        for(int i = 0; i < texts.size(); i++){
            if(texts.get(i).IsOverlapping(_x, _y))
            {
                texts.remove(i);
                UpdateTexts(texts);
            }
        }
        for(int i = 0; i < buttons.size(); i++){
            if(buttons.get(i).IsOverlapping(_x, _y))
            {
                buttons.get(i).linkedTo.RemoveButton(buttons.get(i));
                buttons.remove(i);
                UpdateButtons(buttons);
            }
        }
        for(int i = 0; i < doors.size(); i++){
            if(doors.get(i).IsOverlapping(_x, _y))
            {
                for(int j = 0; j < buttons.size(); j++){
                    buttons.get(j).RemoveLink(doors.remove(i));
                }
                doors.remove(i);
                UpdateDoors(doors);
            }
        }
        if(spawn != null)
        {
            if(spawn.IsOverlapping(_x, _y))
            {
                spawn = null;
            }
        }
    }
}
