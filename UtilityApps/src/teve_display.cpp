// $Id: display.cpp 590 2013-06-03 17:02:43Z markus.frank $
//====================================================================
//  AIDA Detector description implementation for LCD
//--------------------------------------------------------------------
//
//  Generic ROOT based geometry display program
// 
//  Author     : M.Frank
//
//====================================================================
#include "DD4hep/Factories.h"
#include "DD4hep/LCDD.h"
#include "DDRec/SurfaceManager.h"

#include "EvNavHandler.h"
#include "MultiView.h"

#include "run_plugin.h"
#include "TRint.h"

#include "TEveGeoNode.h"
#include "TEveBrowser.h"
#include "TGNumberEntry.h"
#include "TGButton.h"
#include "TGLabel.h"
#include "TStyle.h"
#include "TGComboBox.h"
#include "TEveManager.h"
#include "TSystem.h"
#include "TGLViewer.h"
#include "TEveViewer.h"
#include "TGLPerspectiveCamera.h"
#include "TGLCamera.h"
#include "TEveStraightLineSet.h"
#include "TSysEvtHandler.h"
//#include "TEvePad.h"
#include <TEveScene.h>
#include <TEveProjectionManager.h>
#include <TEveProjectionAxes.h>
#include <TEveWindow.h>

#include "TGeoManager.h"
//#include "TGLUtil.h"
#include "TGLClip.h"
#include "TMap.h"
#include "TObjString.h"

// #define private public
// #include "TEveManager.h"

#include "TGeoShape.h"

using namespace DD4hep ;
using namespace DDRec ;
using namespace Geometry ;
using namespace DDSurfaces ;

//=====================================================================================
// function declarations: 
void next_event();
void make_gui();

TEveStraightLineSet* getSurfaces(int col=kRed, const SurfaceType& type=SurfaceType() ) ;
TEveStraightLineSet* getSurfaceVectors( ) ;

//=====================================================================================

static long teve_display(LCDD& lcdd, int /* argc */, char** /* argv */) {

  TGeoManager* mgr = &lcdd.manager();
  TEveManager::Create();

  //  gEve->fGeometries->Add(new TObjString("DefaultGeometry"),mgr);

  TEveGeoTopNode* tn = new TEveGeoTopNode(mgr, mgr->GetTopNode());
  tn->SetVisLevel(4);

  EvNavHandler *fh = new EvNavHandler;

  // // ---- try to set transparency - does not seem to work ...
  // TGeoNode* node1 = gGeoManager->GetTopNode();
  // int k_nodes = node1->GetNdaughters();
  // for(int i=0; i<k_nodes; i++) {
  //   TGeoNode * CurrentNode = node1->GetDaughter(i);
  //   CurrentNode->GetMedium()->GetMaterial()->SetTransparency(80);
  //   // TEveGeoNode CurrentNode( node1->GetDaughter(i) ) ;
  //   // CurrentNode.SetMainTransparency( 80 ) ;
  // }
  // this code seems to have no effect either ...
  // tn->CSCApplyMainTransparencyToAllChildren() ;
  // tn->SetMainTransparency( 80 ) ;


  gEve->AddGlobalElement( tn );

  TEveElement* surfaces = getSurfaces(  kRed, SurfaceType( SurfaceType::Sensitive ) ) ;
  TEveElement* helperSurfaces = getSurfaces(  kGray, SurfaceType( SurfaceType::Helper ) ) ;
  TEveElement* surfaceVectors = getSurfaceVectors() ;

  gEve->AddGlobalElement( surfaces ) ;
  gEve->AddGlobalElement( helperSurfaces ) ;
  gEve->AddGlobalElement( surfaceVectors ) ;

  TGLViewer *v = gEve->GetDefaultGLViewer();
  // v->GetClipSet()->SetClipType(TGLClip::kClipPlane);
  //  v->ColorSet().Background().SetColor(kMagenta+4);
  v->ColorSet().Background().SetColor(kWhite);
  v->SetGuideState(TGLUtil::kAxesEdge, kTRUE, kFALSE, 0);
  v->RefreshPadEditor(v);
  //  v->CurrentCamera().RotateRad(-1.2, 0.5);

  MultiView::instance()->ImportGeomRPhi( surfaces );
  MultiView::instance()->ImportGeomRhoZ( surfaces ) ;
  MultiView::instance()->ImportGeomRPhi( helperSurfaces );
  MultiView::instance()->ImportGeomRhoZ( helperSurfaces ) ;


  make_gui();

  next_event();

  gEve->FullRedraw3D(kTRUE);

  return 1;
}
DECLARE_APPLY(DD4hepTEveDisplay,teve_display)


//=====================================================================================================================

int main(int argc,char** argv)  {
  return main_default("DD4hepTEveDisplay",argc,argv);
}

//=====================================================================================================================


TEveStraightLineSet* getSurfaceVectors() {

  TEveStraightLineSet* ls = new TEveStraightLineSet("SurfaceVectors");

  LCDD& lcdd = LCDD::getInstance();

  DetElement world = lcdd.world() ;

  // create a list of all surfaces in the detector:
  SurfaceManager surfMan(  world ) ;

  const SurfaceList& sL = surfMan.surfaceList() ;

  for( SurfaceList::const_iterator it = sL.begin() ; it != sL.end() ; ++it ){

    ISurface* surf = *it ;
    const DDSurfaces::Vector3D& u = surf->u() ;
    const DDSurfaces::Vector3D& v = surf->v() ;
    const DDSurfaces::Vector3D& n = surf->normal() ;
    const DDSurfaces::Vector3D& o = surf->origin() ;

    DDSurfaces::Vector3D ou = o + u ;
    DDSurfaces::Vector3D ov = o + v ;
    DDSurfaces::Vector3D on = o + n ;
 
    ls->AddLine( o.x(), o.y(), o.z(), ou.x() , ou.y() , ou.z()  ) ;
    ls->AddLine( o.x(), o.y(), o.z(), ov.x() , ov.y() , ov.z()  ) ;
    ls->AddLine( o.x(), o.y(), o.z(), on.x() , on.y() , on.z()  ) ;

    ls->AddMarker(  o.x(), o.y(), o.z() );
  }
  ls->SetLineColor( kGreen ) ;
  ls->SetMarkerColor( kBlue ) ;
  ls->SetMarkerSize(1);
  ls->SetMarkerStyle(4);
  
  //  gEve->AddElement(ls);

  return ls;
}
//=====================================================================================
TEveStraightLineSet* getSurfaces(int col, const SurfaceType& type) {

  TEveStraightLineSet* ls = new TEveStraightLineSet("Surfaces");

  LCDD& lcdd = LCDD::getInstance();

  DetElement world = lcdd.world() ;

  // create a list of all surfaces in the detector:
  SurfaceManager surfMan(  world ) ;

  const SurfaceList& sL = surfMan.surfaceList() ;

  //  std::cout << " getSurfaces() - #surfaces : " << sL.size() << std::endl ;

  for( SurfaceList::const_iterator it = sL.begin() ; it != sL.end() ; ++it ){

    Surface* surf = *it ;

    if( ! ( surf->type().isVisible() && ( surf->type().isSimilar( type ) ) ) ) 
      continue ;

    const std::vector< std::pair<Vector3D,Vector3D> > lines = surf->getLines() ;

    if( lines.empty() )
      std::cout << " **** drawSurfaces() : empty lines vector for surface " << *surf << std::endl ;

    unsigned nL = lines.size() ;

    for( unsigned i=0 ; i<nL ; ++i){

      //      std::cout << " **** drawSurfaces() : draw line for surface " <<   lines[i].first << " - " <<  lines[i].second  << std::endl ;

      ls->AddLine( lines[i].first.x(),  lines[i].first.y(),  lines[i].first.z(), 
		   lines[i].second.x(), lines[i].second.y(), lines[i].second.z() ) ;
    }
    
    ls->SetLineColor( col ) ;
    ls->SetMarkerColor( col ) ;
    ls->SetMarkerSize(.1);
    ls->SetMarkerStyle(4);

  }

  return ls;
}

//=====================================================================================

void make_gui() {

   // Create minimal GUI for event navigation.

   TEveBrowser* browser = gEve->GetBrowser();
   browser->StartEmbedding(TRootBrowser::kLeft);

   TGMainFrame* frmMain = new TGMainFrame(gClient->GetRoot(), 1000, 600);
   frmMain->SetWindowName("DD4hep GUI");
   frmMain->SetCleanup(kDeepCleanup);

   TGHorizontalFrame* hf = new TGHorizontalFrame(frmMain);
   {
      
      TString icondir( Form("%s/icons/", gSystem->Getenv("ROOTSYS")) );
      TGPictureButton* b = 0;
      EvNavHandler    *fh = new EvNavHandler;

      b = new TGPictureButton(hf, gClient->GetPicture(icondir+"GoBack.gif"));
      b->SetEnabled(kFALSE);
      b->SetToolTipText("Go to previous event - not supported.");
      hf->AddFrame(b);
      b->Connect("Clicked()", "EvNavHandler", fh, "Bck()");

      b = new TGPictureButton(hf, gClient->GetPicture(icondir+"GoForward.gif"));
      b->SetToolTipText("Generate new event.");
      hf->AddFrame(b);
      b->Connect("Clicked()", "EvNavHandler", fh, "Fwd()");
   }
   frmMain->AddFrame(hf);

   frmMain->MapSubwindows();
   frmMain->Resize();
   frmMain->MapWindow();

   browser->StopEmbedding();
   browser->SetTabTitle("Event Control", 0);
}
//=====================================================================================

