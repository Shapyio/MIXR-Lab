
#include "mixr/graphics/Page.hpp"
#include "mixr/graphics/Display.hpp"
#include "mixr/base/PairStream.hpp"
#include "mixr/base/numeric/Number.hpp"
#include "mixr/base/Pair.hpp"

namespace mixr {
namespace graphics {

IMPLEMENT_SUBCLASS(Page, "Page")

BEGIN_SLOTTABLE(Page)
    "page",                // 1: Initial subpage
    "pages",               // 2: Subpages
    "pagingEvent",         // 3: Page change event
    "subpagesFirst",       // 4: Draw subpages first (default: draw our page graphics first)
    "focusSlavedToSubpage", // 5: Slave the focus to the subpage (default: true)
END_SLOTTABLE(Page)

BEGIN_SLOT_MAP(Page)
    ON_SLOT(1, setSlotPage,                 base::Identifier)
    ON_SLOT(2, setSlotSubpageStream,        base::PairStream)
    ON_SLOT(2, setSlotSubpageSingle,        Page)
    ON_SLOT(3, setSlotPagingEvent,          base::PairStream)
    ON_SLOT(4, setSlotDrawSubpageFirst,     base::Number)
    ON_SLOT(5, setSlotFocusSlavedToSubpage, base::Number)
END_SLOT_MAP()

BEGIN_EVENT_HANDLER(Page)
    ON_EVENT(ON_ENTRY,onEntry)
    ON_EVENT(ON_EXIT,onExit)
    ON_EVENT_OBJ(BUTTON_HIT,onButtonHit,base::String)
    ON_ANYKEY(onKeyHit)
END_EVENT_HANDLER()

Page::Page()
{
   STANDARD_CONSTRUCTOR()
}

void Page::copyData(const Page& org, const bool cc)
{
    BaseClass::copyData(org);

    // Copy subpages
    if (!cc && subpages != nullptr) { subpages->unref(); }
    if (org.subpages != nullptr) {
        subpages = org.subpages->clone();
        processSubpages();
    }
    else subpages = nullptr;

    // Copy events
    if (!cc && pageChgEvents != nullptr) { pageChgEvents->unref(); }
    if (org.pageChgEvents != nullptr) {
        pageChgEvents = org.pageChgEvents->clone();
    }
    else pageChgEvents = nullptr;

    // select the same pages
    cpName = org.cpName;
    cp = nullptr;
    np = nullptr;

    // Clear the subpage stack
    clearSubpageStack();

    // Post draw flag
    postDraw1 = org.postDraw1;

   focusSlavedToSubpage = org.focusSlavedToSubpage;

    // At this time we are not copying the paging arguments.
    pageArg = nullptr;
    caller = nullptr;
}

void Page::deleteData()
{
    // De-select the current pages
    cp = nullptr;
    cpName.empty();

    // Delete list of subpages
    if (subpages != nullptr) subpages->unref();
    subpages = nullptr;

    // Delete list of events
    if (pageChgEvents != nullptr) pageChgEvents->unref();
    pageChgEvents = nullptr;
}

//------------------------------------------------------------------------------
// updateTC() -- Update time critical stuff here
//------------------------------------------------------------------------------
void Page::updateTC(const double dt)
{
   // Update our subpage and base class
   if (cp != nullptr) cp->tcFrame(dt);
   BaseClass::updateTC(dt);
}

//------------------------------------------------------------------------------
// updateData() --
//------------------------------------------------------------------------------
void Page::updateData(const double dt)
{
   // ---
   // Prime the pump -- when we don't have a current subpage (cp) and we do
   // have a subpage name (cpName) then make it our current subpage.
   // ---
   if (cp == nullptr && !cpName.isEmpty()) {
      base::Pair* p = nullptr;
      if (subpages != nullptr) p = subpages->findByName(cpName);
      if (p != nullptr) {
         np = static_cast<Page*>(p->object());
         np->caller  = nullptr;
         np->pageArg = nullptr;
      }
   }

   // ---
   // Check for page changes
   // ---
   if (np != nullptr) {
      if ( np != cp ) {
         // When we have a new page ...
         //  1) inform the old page of EXIT and remove focus
         //  2) set the current page pointer
         //  3) inform the new page of the ENTRY and set focus
         if (cp != nullptr) {
            cp->event(ON_EXIT);
            if (isFocusSlavedToSubpage()) getDisplay()->focus(this);
         }
         cp = np;
         if (cp != nullptr) {
            if (isFocusSlavedToSubpage()) getDisplay()->focus(cp);
            cp->event(ON_ENTRY);
         }
      }
   }

   // Update our subpage and base class
   if (cp != nullptr) cp->updateData(dt);
   BaseClass::updateData(dt);
}

//------------------------------------------------------------------------------
// reset() -- Reset parameters
//------------------------------------------------------------------------------
void Page::reset()
{
    BaseClass::reset();
    if (subpages != nullptr) {
        // Reset all of our sub-pages
        base::List::Item* item = subpages->getFirstItem();
        while (item != nullptr) {
            const auto pair = static_cast<base::Pair*>(item->getValue());
            const auto obj = static_cast<Component*>(pair->object());
            if (obj != nullptr) obj->reset();
            item = item->getNext();
        }
    }
}

//------------------------------------------------------------------------------
// findBySelectName() -- find one of our components by its GL Select (pick) name
//                    (our children first then grandchildren)
//------------------------------------------------------------------------------
base::Pair* Page::findBySelectName(const GLuint name)
{
    // Use our base class's functions to check normal components
    base::Pair* q = Graphic::findBySelectName(name);

    // If still not found, check our subpage ...
    if (q == nullptr && cp != nullptr) {
        q = cp->findBySelectName(name);
    }
    return q;
}

//------------------------------------------------------------------------------
// newSubpage() -- change subpages
//------------------------------------------------------------------------------
bool Page::newSubpage(Page* const np1, Page* theCaller, base::Object* theArg)
{
    bool ok = false;
    if (np1 != nullptr) {
        cpName.empty();       // Unknown name
        np = np1;
        np->pageArg = theArg;
        np->caller  = theCaller;
        clearSubpageStack();
        ok = true;
    }
    return ok;
}

//------------------------------------------------------------------------------
// newSubpage() -- change subpages by name; returns true of page was found
//------------------------------------------------------------------------------
bool Page::newSubpage(const char* const name, Page* theCaller, base::Object* theArg)
{
    bool ok = false;
    base::Pair* p = nullptr;
    if (subpages != nullptr) p = subpages->findByName(name);
    if (p != nullptr) {
        cpName = name;            // It's our page
        np = static_cast<Page*>(p->object());
        np->pageArg = theArg;
        np->caller  = theCaller;
        clearSubpageStack();
        ok = true;
    }
    return ok;
}

//------------------------------------------------------------------------------
// clearSubpageStack() -- Clear the subpage stack
//------------------------------------------------------------------------------
bool Page::clearSubpageStack()
{
   subpageSP = SUBPAGE_STACK_SIZE;
   return true;
}

//------------------------------------------------------------------------------
// pushSubpage() -- push the current subpage and change to new subpage 'name'.
//------------------------------------------------------------------------------
bool Page::pushSubpage(const char* const name, Page* theCaller, base::Object* theArg)
{
    bool ok = false;
    if (subpageSP > 0) {
        Page* currPage = cp;
        base::Pair* p = nullptr;
        if (subpages != nullptr) p = subpages->findByName(name);
        if (p != nullptr) {
           cpName = name;            // It's our page
           np = static_cast<Page*>(p->object());
           np->pageArg = theArg;
           np->caller  = theCaller;
           ok = true;
        }
        //ok = newSubpage(name,theCaller,theArg);
        if (ok) subpageStack[--subpageSP] = currPage;
    }
    return ok;
}

//------------------------------------------------------------------------------
// popSubpage() -- pop to the previous subpage on the stack
//------------------------------------------------------------------------------
bool Page::popSubpage(Page* theCaller, base::Object* theArg)
{
   bool ok = false;
   if (subpageSP < SUBPAGE_STACK_SIZE) {
      //ok = newSubpage(subpageStack[subpageSP++],theCaller,theArg);
      cpName.empty();               // Unknown name
      np = subpageStack[subpageSP++];
      np->pageArg = theArg;
      np->caller  = theCaller;
      ok = true;
   }
   return ok;
}

//------------------------------------------------------------------------------
// Functions that ask our container to change pages (we're outa here)
//------------------------------------------------------------------------------

// New page by pointer
bool Page::newPage(Page* const newPage, Page* theCaller, base::Object* theArg)
{
    bool ok = false;
    const auto cc = dynamic_cast<Page*>(container());
    if (cc != nullptr) ok = cc->newSubpage(newPage,theCaller,theArg);
    return ok;
}

// New page by name
bool Page::newPage(const char* const name, Page* theCaller, base::Object* theArg)
{
    bool ok = false;
    const auto cc = dynamic_cast<Page*>(container());
    if (cc != nullptr) ok = cc->newSubpage(name,theCaller,theArg);
    return ok;
}

// Push new page
bool Page::pushPage(const char* const name, Page* theCaller, base::Object* theArg)
{
    bool ok = false;
    const auto cc = dynamic_cast<Page*>(container());
    if (cc != nullptr) ok = cc->pushSubpage(name,theCaller,theArg);
    return ok;
}

// Pop back to previous page
bool Page::popPage(Page* theCaller, base::Object* theArg)
{
    bool ok = false;
    const auto cc = dynamic_cast<Page*>(container());
    if (cc != nullptr) ok = cc->popSubpage(theCaller,theArg);
    return ok;
}

//------------------------------------------------------------------------------
// findSubpageByName(), findSubpageByType() -- find a member
//------------------------------------------------------------------------------
base::Pair* Page::findSubpageByName(const char* const slotname)
{
    base::Pair* p = nullptr;
    if (subpages != nullptr) p = subpages->findByName(slotname);
    return p;
}


base::Pair* Page::findSubpageByType(const std::type_info& type)
{
    base::Pair* p = nullptr;
    if (subpages != nullptr) p = subpages->findByType(type);
    return p;
}


//------------------------------------------------------------------------------
// onButtonHit() -- handle button hits (with button name) as page change requests
//------------------------------------------------------------------------------
bool Page::onButtonHit(const base::String* const obhobj)
{
    bool used = false;
    if (obhobj != nullptr && pageChgEvents != nullptr) {
        used = true;
        base::Pair* pageEvent = pageChgEvents->findByName(*obhobj);
        if (pageEvent != nullptr) {
            const auto id = dynamic_cast<base::Identifier*>(pageEvent->object());
            if (id != nullptr) {
                // Find our container and the new page ID, then push
                // current page and go to new page
                newPage(*id, this);
            }
        }
    }
    return used;
}


//------------------------------------------------------------------------------
// onKeyHit() -- handle keyboard inputs as page change requests
//------------------------------------------------------------------------------
bool Page::onKeyHit(const int key)
{
    bool used = false;
    if (pageChgEvents != nullptr) {
        char keyName[2];
        keyName[0] = char(key);
        keyName[1] = '\0';

        // search for a page change event
        base::Pair*pageEvent = pageChgEvents->findByName(keyName);
        if (pageEvent != nullptr) {
            const auto id = dynamic_cast<base::Identifier*>(pageEvent->object());
            if (id != nullptr) {
                // Find our container and the new page ID, then push
                // current page and go to new page
                newPage(*id,this);
                used = true;
            }
        }
    }
    return used;
}

//------------------------------------------------------------------------------
// onEntry() --
//------------------------------------------------------------------------------
bool Page::onEntry()
{
   // send our focus to our subpage, if we have one.
   if (subpage() != nullptr) {
      getDisplay()->focus( subpage() );
      subpage()->event(ON_ENTRY);
   }
   return true;
}

//------------------------------------------------------------------------------
// onExit() --
//------------------------------------------------------------------------------
bool Page::onExit()
{
   if (subpage() != nullptr) {
      subpage()->event(ON_EXIT);
   }
   return true;
}


//------------------------------------------------------------------------------
// draw -- draw this object and its children
//------------------------------------------------------------------------------
void Page::draw()
{
   // Only when this object is visible ...
   if ( !isVisible() ) return;

    if (postDraw1) {
        // Draw the current subpage and ...
        if (cp != nullptr) cp->draw();
        // then draw our own page components
        BaseClass::draw();
    }
    else {
        // Draw our own page components and ...
        BaseClass::draw();
        // then draw the current subpage
        if (cp != nullptr) cp->draw();
    }

}

//------------------------------------------------------------------------------
// processSubpages() -- process our subpages; make sure they are all of
// type Page (or derived from it)and tell them that we are their
// container.
//------------------------------------------------------------------------------
bool Page::processSubpages()
{
    bool ok = true;
    if (subpages != nullptr) {
        // Make sure we have only Pages and tell all of the pages
        // that we are their container.
        const base::List::Item* item = subpages->getFirstItem();
        while (ok && item != nullptr) {
            const auto p = const_cast<base::Pair*>(static_cast<const base::Pair*>(item->getValue()));
            item = item->getNext();
            const auto g = dynamic_cast<Page*>(p->object());
            if (g != nullptr) {
                // It MUST be of type Page
                g->container(this);
            }
            else {
                // Delete components that are not of Page type
                if (isMessageEnabled(MSG_ERROR)) {
                    std::cerr << "Page::processSubpages(): " << *p->slot() << " is not a Page!" << std::endl;
                }
                ok = false;
            }
        }
    }
    return ok;
}

//------------------------------------------------------------------------------
// setSlotPage() -- sets the initial page
//------------------------------------------------------------------------------
bool Page::setSlotPage(const base::Identifier* const pobj)
{
    if (pobj != nullptr) cpName =  *pobj;
    return true;
}

//------------------------------------------------------------------------------
//  setSlotSubpageStream() -- it takes a pair stream
//------------------------------------------------------------------------------
bool Page::setSlotSubpageStream (base::PairStream* const psobj)
{
    bool ok = false;
    if (psobj != nullptr) {
        // When a PairStream (i.e., more than one, a list) of pages
        if (subpages != nullptr) subpages->unref();
        subpages = psobj;
        subpages->ref();

        ok = processSubpages();
    }
    return ok;
}

//------------------------------------------------------------------------------
//  setSlotSubpageSingle() -- it takes an Page
//------------------------------------------------------------------------------
bool Page::setSlotSubpageSingle(Page* const pobj)
{
    bool ok = false;
    if (pobj != nullptr) {
        if (subpages != nullptr) subpages->unref();

        subpages = new base::PairStream();
        subpages->put( new base::Pair("1",pobj) );
        ok = processSubpages();
    }
    return ok;
}

//------------------------------------------------------------------------------
// setSlotPagingEvent() -- sets the page change event
//------------------------------------------------------------------------------
bool Page::setSlotPagingEvent(base::PairStream* const peobj)
{
    if (peobj != nullptr) {
        if (pageChgEvents != nullptr) pageChgEvents->unref();
        pageChgEvents = peobj;
        pageChgEvents->ref();
    }
    return true;
}

//------------------------------------------------------------------------------
// setSlotDrawSubpageFirst() -- Draw subpages first (default: draw our page graphics first)
//------------------------------------------------------------------------------
bool Page::setSlotDrawSubpageFirst(const base::Number* const spfobj)
{
    if (spfobj != nullptr) postDraw1 = spfobj->getBoolean();
    return true;
}

//------------------------------------------------------------------------------
// drawSubpageFirst() -- Draw subpages first (default: draw our page graphics first)
//------------------------------------------------------------------------------
bool Page::setSlotFocusSlavedToSubpage(const base::Number* const msg)
{
    if (msg != nullptr) setFocusSlavedToSubpage( msg->getBoolean() );
    return true;
}

}
}
