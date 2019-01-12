
#ifndef _USER_EVENTS_H_
#define _USER_EVENTS_H_

#include "event.h"
#include "glo_def.h"
#include "eventtype.h"

#include <string>
#include <vector>

// class MobileCodeReqEv : public iEvent
// {
// public:
//     MobileCodeReqEv(const std::string& mobile) :
//         iEvent(EEVENTID_GET_MOBILE_CODE_REQ, generateSeqNo()), mobile_(mobile)
//     {};

//     const std::string& get_mobile(){return mobile_;};
//     virtual std::ostream& dump(std::ostream& out) const;
// private:
//     std::string mobile_;
// };

// class LoginReqEv : public iEvent
// {
// public :
// 	LoginReqEv(const std::string& mobile, const std::string& code) :
//         iEvent(EEVENTID_LOGIN_REQ, generateSeqNo()), mobile_(mobile), code_(code)
// 	{};

//     const std::string& get_mobile(){return mobile_;};
//     const std::string& get_code(){return code_;};
//     virtual std::ostream& dump(std::ostream& out) const;
// private :
//     std::string mobile_;
//     std::string code_;
// };


#endif//_USER_EVENTS_H_

