using System;
using Microsoft.Xna.Framework;
using StardewModdingAPI;
using StardewModdingAPI.Events;
using StardewValley;
using StardewValley.TerrainFeatures;

namespace MyCustomSprinkler
{
    public class ModEntry : Mod
    {
        public override void Entry(IModHelper helper)
        {
            // ดักจับ Event ตอนเช้าวันใหม่เริ่มต้นขึ้น (ช่วงที่สปริงเกอร์ทำงาน)
            helper.Events.GameLoop.DayStarted += OnDayStarted;

            // ดักจับ Event ตอนที่ผู้เล่นโหลดเซฟเข้าเกมสำเร็จ เพื่อเพิ่มสูตรคราฟต์และแจ้งเตือน
            helper.Events.GameLoop.SaveLoaded += OnSaveLoaded;
        }

        private void OnSaveLoaded(object? sender, SaveLoadedEventArgs e)
        {
            // ตรวจสอบว่าผู้เล่นคนนี้มีสูตรคราฟต์นี้หรือยัง ถ้ายังไม่มีให้เพิ่มเข้าไปในระบบของตัวละคร
            if (!Game1.player.craftingRecipes.ContainsKey("My Custom Sprinkler"))
            {
                Game1.player.craftingRecipes.Add("My Custom Sprinkler", 0);
            }

            // แจ้งเตือนเมื่อโหลดเซฟเสร็จ (รหัสสี 1 = ข้อความแจ้งเตือนทั่วไปสีส้ม/แดง)
            Game1.addHUDMessage(new HUDMessage("มอดสปริงเกอร์ 16x16 เปิดใช้งานแล้ว!", 1));
        }

        private void OnDayStarted(object? sender, DayStartedEventArgs e)
        {
            bool hasSprinklerWorked = false;

            foreach (GameLocation location in Game1.locations)
            {
                foreach (var pair in location.Objects.Pairs)
                {
                    StardewValley.Object obj = pair.Value;

                    if (obj.QualifiedItemId == "(O)MyCustomSprinkler_Item")
                    {
                        WaterCustomArea(location, pair.Key);
                        hasSprinklerWorked = true; 
                    }
                }
            }

            // แก้ไขจุดนี้: เรียกใช้ Constructor แบบเรียบง่ายที่สุด ไม่ระบุ noFade หรือ timeLeft
            // เลข 2 คือ รหัสข้อความประเภท "ความสำเร็จ/ปลดล็อก" ซึ่งจะแสดงผลที่มุมซ้ายล่างอย่างสวยงาม
            if (hasSprinklerWorked)
            {
                Game1.addHUDMessage(new HUDMessage("สปริงเกอร์ประดิษฐ์ 16x16 ได้รดน้ำฟาร์มของคุณแล้ว!", 2));
            }
        }

        private void WaterCustomArea(GameLocation location, Vector2 sprinklerPos)
        {
            for (int x = -8; x <= 8; x++)
            {
                for (int y = -8; y <= 8; y++)
                {
                    Vector2 tile = new Vector2(sprinklerPos.X + x, sprinklerPos.Y + y);

                    if (location.terrainFeatures.TryGetValue(tile, out TerrainFeature feature) && feature is HoeDirt dirt)
                    {
                        dirt.state.Value = HoeDirt.watered;
                    }
                }
            }
        }
    }
}
