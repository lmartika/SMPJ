//Author K. Kousouris
//Modified by: S. Ganguly

#ifndef QCDPFJetBTag_h
#define QCDPFJetBTag_h
#include "SMPJ/AnalysisFW/interface/QCDJet.h"
#include "TLorentzVector.h"
class QCDPFJetBTag : public QCDJet {
   public:
     //------------ Constructor ------------------------------
     QCDPFJetBTag() {}
     //------------ Destructor -------------------------------
     ~QCDPFJetBTag() {}
     //------------ Set methods ------------------------------
     void setPositiveNegativeCSV(float fcsvpfpositive, float fcsvpfnegative) { CSVpfPositive_ = fcsvpfpositive; CSVpfNegative_ = fcsvpfnegative;}

     void setTagRecommended(float recommend1, float recommend2, float recommend3) { recommend1_ = recommend1; recommend2_ = recommend2; recommend3_ = recommend3; } 

     void setDeepCSV (float DeepCSVb, float
             DeepCSVc, float DeepCSVl, float DeepCSVbb , float DeepCSVcc , float DeepCSVbN ,
             float DeepCSVcN , float DeepCSVlN , float DeepCSVbbN, float DeepCSVccN, float
             DeepCSVbP , float DeepCSVcP , float DeepCSVlP , float DeepCSVbbP, float
             DeepCSVccP) {
         DeepCSVb_ = DeepCSVb;
         DeepCSVc_ = DeepCSVc;
         DeepCSVl_ = DeepCSVl;
         DeepCSVbb_ = DeepCSVbb;
         DeepCSVcc_ = DeepCSVcc;
         DeepCSVbN_ = DeepCSVbN;
         DeepCSVcN_ = DeepCSVcN;
         DeepCSVlN_ = DeepCSVlN;
         DeepCSVbbN_ = DeepCSVbbN;
         DeepCSVccN_ = DeepCSVccN;
         DeepCSVbP_ = DeepCSVbP;
         DeepCSVcP_ = DeepCSVcP;
         DeepCSVlP_ = DeepCSVlP;
         DeepCSVbbP_ = DeepCSVbbP;
         DeepCSVccP_ = DeepCSVccP;}


     void setFlavour(float fpartonflavour, float fhadronflavour, float fpartonflavourPhysicsDef) {partonFlavour_ = fpartonflavour; hadronFlavour_ = fhadronflavour; partonFlavourPhysicsDef_ = fpartonflavourPhysicsDef;}

     void setQGTagger(float fQGTagger) {QGtagger_ = fQGTagger;}

     void setBoosted(float fboosted) {boosted_ = fboosted;}
     void setCTagger(float fpfCombinedCvsL, float fpfCombinedCvsB) {pfCombinedCvsL_ = fpfCombinedCvsL; pfCombinedCvsB_ = fpfCombinedCvsB;}

     float csvpfpositive()      const {return CSVpfPositive_;}
     float csvpfnegative()      const {return CSVpfNegative_;}
    
     float pfBoostedDouble()  const {return boosted_;} 

     float partonflavour()      const {return partonFlavour_;}
     float partonflavourPhysicsDef()      const {return partonFlavourPhysicsDef_;}
     float hadronflavour()      const {return hadronFlavour_;}

     float qgtagger()      const {return QGtagger_;}

     float pfJetProbabilityBJetTags() const {return recommend1_;}
     float pfCombinedInclusiveSecondaryVertexV2BJetTags() const {return recommend2_;}
     float pfCombinedMVAV2BJetTags() const {return recommend2_;}
     float pfCombinedCvsL() const {return pfCombinedCvsL_;}
     float pfCombinedCvsB() const {return pfCombinedCvsB_;}

   private:
     float CSVpfPositive_;
     float CSVpfNegative_;
     
     float boosted_;
     float recommend1_;
     float recommend2_;
     float recommend3_;
     // ctaggers
     float pfCombinedCvsL_;
     float pfCombinedCvsB_;
     float DeepCSVb_, DeepCSVc_, DeepCSVl_, DeepCSVbb_, DeepCSVcc_, DeepCSVbN_,
           DeepCSVcN_, DeepCSVlN_, DeepCSVbbN_, DeepCSVccN_,  DeepCSVbP_, DeepCSVcP_,
           DeepCSVlP_, DeepCSVbbP_, DeepCSVccP_;

     float QGtagger_;

     float partonFlavour_;
     float partonFlavourPhysicsDef_;
     float hadronFlavour_;
};
#endif
