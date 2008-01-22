/** \file
 * Implementation of class RawDataCollectorModule
 *
 */

#include "EventFilter/RawDataCollector/interface/RawDataCollectorModule.h"
#include <DataFormats/FEDRawData/interface/FEDRawDataCollection.h> 
#include <DataFormats/FEDRawData/interface/FEDRawData.h>
#include <DataFormats/FEDRawData/interface/FEDNumbering.h>

#include <DataFormats/Common/interface/Handle.h>
#include <FWCore/Framework/interface/Event.h>


#include <iostream>

using namespace edm;

RawDataCollectorModule::RawDataCollectorModule(const edm::ParameterSet& pset) {

  produces<FEDRawDataCollection>();
}

RawDataCollectorModule::~RawDataCollectorModule(){

}


void RawDataCollectorModule::produce(Event & e, const EventSetup& c){
 
 /// Get Data from all FEDs
 std::vector< Handle<FEDRawDataCollection> > rawData;
 e.getManyByType(rawData);

 std::auto_ptr<FEDRawDataCollection> producedData(new FEDRawDataCollection);

 for (unsigned int i=0; i< rawData.size(); ++i ) { 

   const FEDRawDataCollection *rdc=rawData[i].product();

   for ( int j=0; j< FEDNumbering::lastFEDId(); ++j ) {
     const FEDRawData & fedData = rdc->FEDData(j);
     size_t size=fedData.size();

     if ( size > 0 ) {
       // this fed has data -- lets copy it
       FEDRawData & fedDataProd = producedData->FEDData(j);
       if ( fedDataProd.size() != 0 ) {
	 std::cout << " More than one FEDRawDataCollection with data in FED ";
	 std::cout << j << " Skipping the 2nd\n";
	 continue;
       } 
       fedDataProd.resize(size);
       unsigned char *dataProd=fedDataProd.data();
       const unsigned char *data=fedData.data();
       for ( unsigned int k=0; k<size; ++k ) {
	 dataProd[k]=data[k];
       }
     }
   }
 }

 // Insert the new product in the event  
 e.put(producedData);  

}





