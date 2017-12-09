/******************************************************************************/
struct _Loader
{
   static Bool BackgroundLoad(Thread &thread)
   {
	  ThreadMayUseGPUData();
      Game::World.update(Cam.at);

      return false;
   }

   Thread thread;

   GUI customGui;
   Progress progress;


   void start()
   {
      customGui += progress.create(Rect_C(Vec2(0, 0), 1, 0.05), PROGRESS_PERCENT);

      thread.create(BackgroundLoad);
   }

   Bool update()
   {
      if(!thread.created()) return false;

      customUpdate();

      return true;
   }

   Bool draw()
   {
      if(!thread.created()) return false;

      customDraw();

      if(!thread.active())
      {
	     progress.del();

         thread.del();

         Renderer.setFade(1.0f);
      }

      return true;
   }

   void customUpdate()
   {
      progress.set(Round(Game::World.updateProgress()), 100);

      customGui.update();

      Time.wait(1000 / 24);
   }

   void customDraw()
   {
      D.clear(BLACK);

	  TextDS tds;

	  tds.font = defaultFont;
	
	  D.text(tds, 0, 0, S + "£adowanie: " + Round(Game::World.updateProgress() * 100) + "%");

	  customGui.draw();
   }
} extern Loader;
/******************************************************************************/