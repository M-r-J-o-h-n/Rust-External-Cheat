#pragma once

namespace Rust {
	class EntityUpdator
	{
	public:
		//main thread functions 
		static void UpdateLocalPlayerAndCameraData();  //This deletes existing list and make whole list again

		//Update thread
		static void UpdateThread();

		//Adder thread functions 
		static void AddNewTaggedObjects();
		static void AddNewActiveObjects();

	private:
		static void UpdateTaggedObject(); //This adds new entities in the list and delete outdated entities
		static void UpdateActiveObject();
	};
}
