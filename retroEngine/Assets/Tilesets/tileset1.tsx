<?xml version="1.0" encoding="UTF-8"?>
<tileset version="1.2" tiledversion="2019.02.10" name="tileset1" tilewidth="16" tileheight="16" tilecount="256" columns="16">
 <properties>
  <property name="color" type="color" value="#ffff55ff"/>
  <property name="file" type="file" value=""/>
  <property name="int" type="int" value="0"/>
 </properties>
 <image source="tileset1.png" width="256" height="256"/>
 <tile id="5">
  <objectgroup draworder="index">
   <object id="2" name="tile" type="collider" x="0" y="0" width="16" height="16"/>
  </objectgroup>
 </tile>
 <tile id="116">
  <animation>
   <frame tileid="116" duration="100"/>
   <frame tileid="117" duration="100"/>
  </animation>
 </tile>
 <tile id="119">
  <objectgroup draworder="index">
   <object id="1" type="grass" x="9.125" y="15.25">
    <point/>
   </object>
  </objectgroup>
 </tile>
</tileset>
